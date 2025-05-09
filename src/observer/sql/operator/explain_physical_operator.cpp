/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/12/27.
//

#include "sql/operator/explain_physical_operator.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "sql/operator/order_by_physical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/table_scan_physical_operator.h"
#include <cstdint>
#include <sstream>

using namespace std;

RC ExplainPhysicalOperator::open(Trx *)
{
  ASSERT(children_.size() == 1, "explain must has 1 child");
  return RC::SUCCESS;
}

RC ExplainPhysicalOperator::close() { return RC::SUCCESS; }

void ExplainPhysicalOperator::generate_physical_plan()
{
  stringstream ss;
  ss << "OPERATOR(NAME)\n";

  int               level = 0;
  std::vector<bool> ends;
  ends.push_back(true);
  const auto children_size = static_cast<int>(children_.size());
  for (int i = 0; i < children_size - 1; i++) {
    to_string(ss, children_[i].get(), level, false /*last_child*/, ends);
  }
  if (children_size > 0) {
    to_string(ss, children_[children_size - 1].get(), level, true /*last_child*/, ends);
  }

  physical_plan_ = ss.str();
}

RC ExplainPhysicalOperator::next()
{
  if (!physical_plan_.empty()) {
    return RC::RECORD_EOF;
  }
  generate_physical_plan();

  vector<Value> cells;
  Value         cell(physical_plan_.c_str());
  cells.emplace_back(cell);
  tuple_.set_cells(cells);
  return RC::SUCCESS;
}

RC ExplainPhysicalOperator::next(Chunk &chunk)
{
  if (!physical_plan_.empty()) {
    return RC::RECORD_EOF;
  }
  generate_physical_plan();

  Value cell(physical_plan_.c_str());
  auto  column = make_unique<Column>();
  column->init(cell);
  chunk.add_column(std::move(column), 0);
  return RC::SUCCESS;
}

Tuple *ExplainPhysicalOperator::current_tuple() { return &tuple_; }

/**
 * 递归打印某个算子
 * @param os 结果输出到这里
 * @param oper 将要打印的算子
 * @param level 当前算子在第几层
 * @param last_child 当前算子是否是当前兄弟节点中最后一个节点
 * @param ends 表示当前某个层级上的算子，是否已经没有其它的节点，以判断使用什么打印符号
 */
void ExplainPhysicalOperator::to_string(
    std::ostream &os, PhysicalOperator *oper, int level, bool last_child, std::vector<bool> &ends)
{
  for (int i = 0; i < level - 1; i++) {
    if (ends[i]) {
      os << "  ";
    } else {
      os << "│ ";
    }
  }

  if (level > 0) {
    if (last_child) {
      os << "└─";
      ends[level - 1] = true;
    } else {
      os << "├─";
    }
  }

  if (oper->type() == PhysicalOperatorType::ORDER_BY &&
      oper->children().front()->type() == PhysicalOperatorType::TABLE_SCAN) {

    auto        table_oper = static_cast<TableScanPhysicalOperator *>(oper->children().front().get());
    auto        table      = table_oper->table();
    auto       &table_meta = table->table_meta();
    std::string ss;
    bool        need_rewrite = false;
    for (int i = 0; i < table_meta.index_num(); ++i) {
      auto        index_meta   = table->table_meta().index(i);
      const auto &index_fields = index_meta->fields();
      if (index_fields.size() == 1 && table_meta.field(index_fields[0].c_str())->type() == AttrType::VECTORS) {
        // os << "VECTOR_INDEX_SCAN(" << index_meta->name() << " ON " << table_meta.name() << ")";
        need_rewrite = true;
        ss           = std::string("VETOR_INDEX_SCAN(") + index_meta->name() + " ON " + table_meta.name() + ")";
      }
    }

    if (need_rewrite) {
      os << ss;
      return;
    } else {
      os << oper->name();
      std::string param = oper->param();
      if (!param.empty()) {
        os << "(" << param << ")";
      }
      os << '\n';
    }
  } else if (oper->type() == PhysicalOperatorType::PROJECT &&
             oper->children().front()->type() == PhysicalOperatorType::ORDER_BY &&
             oper->children().front()->children().front()->type() == PhysicalOperatorType::TABLE_SCAN) {
    auto  table_oper   = static_cast<TableScanPhysicalOperator *>(oper->children().front()->children().front().get());
    auto  table        = table_oper->table();
    auto &table_meta   = table->table_meta();
    bool  need_rewrite = false;
    std::string ss;
    for (int i = 0; i < table_meta.index_num(); ++i) {
      auto        index_meta   = table->table_meta().index(i);
      const auto &index_fields = index_meta->fields();
      if (index_fields.size() == 1 && table_meta.field(index_fields[0].c_str())->type() == AttrType::VECTORS) {
        need_rewrite = true;
        ss           = std::string("VECTOR_INDEX_SCAN(") + index_meta->name() + " ON " + table_meta.name() + ")";
        break;
      }
    }

    if (need_rewrite) {
      os << oper->name();
      std::string param = oper->param();
      if (!param.empty()) {
        os << "(" << param << ")";
      }
      os << '\n';
      ++level;

      for (int i = 0; i < level - 1; i++) {
        if (ends[i]) {
          os << "  ";
        } else {
          os << "│ ";
        }
      }
      os << "└─" << ss;
      return;
    } else {
      os << oper->name();
      std::string param = oper->param();
      if (!param.empty()) {
        os << "(" << param << ")";
      }
      os << '\n';
      auto project_oper = static_cast<ProjectPhysicalOperator *>(oper);
      if (project_oper->limit() != INT32_MAX) {
        ++level;
        for (int i = 0; i < level - 1; i++) {
          if (ends[i]) {
            os << "  ";
          } else {
            os << "│ ";
          }
        }
        os << "└─" << "LIMIT(" << project_oper->limit() << ")\n";
      }
    }
  } else if (oper->type() == PhysicalOperatorType::PROJECT) {
    os << oper->name();
    std::string param = oper->param();
    if (!param.empty()) {
      os << "(" << param << ")";
    }
    os << '\n';
    auto project_oper = static_cast<ProjectPhysicalOperator *>(oper);
    if (project_oper->limit() != INT32_MAX) {
      ++level;
      for (int i = 0; i < level - 1; i++) {
        if (ends[i]) {
          os << "  ";
        } else {
          os << "│ ";
        }
      }
      os << "└─" << "LIMIT(" << project_oper->limit() << ")\n";
    }
  } else {
    os << oper->name();
    std::string param = oper->param();
    if (!param.empty()) {
      os << "(" << param << ")";
    }
    os << '\n';
  }

  if (static_cast<int>(ends.size()) < level + 2) {
    ends.resize(level + 2);
  }
  ends[level + 1] = false;

  auto      &children = oper->children();
  const auto size     = static_cast<int>(children.size());
  for (auto i = 0; i < size - 1; i++) {
    to_string(os, children[i].get(), level + 1, false /*last_child*/, ends);
  }

  if (size > 0) {
    to_string(os, children[size - 1].get(), level + 1, true /*last_child*/, ends);
  }
}
