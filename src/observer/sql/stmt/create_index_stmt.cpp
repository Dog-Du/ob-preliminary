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
// Created by Wangyunlai on 2023/4/25.
//

#include "sql/stmt/create_index_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace common;

RC CreateIndexStmt::create(Db *db, const CreateIndexSqlNode &create_index, Stmt *&stmt)
{
  stmt = nullptr;

  const char *table_name = create_index.relation_name.c_str();
  if (is_blank(table_name) || is_blank(create_index.index_name.c_str()) ||
      create_index.attrs.empty() || is_blank(create_index.attrs.front().c_str())) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, index name=%s, attribute name=%s",
        db, table_name, create_index.index_name.c_str(), (create_index.attrs.empty() ? "nothing" : create_index.attrs.front().c_str()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 如果索引字段有重复，返回错误
  // 如果索引字段有不合法，返回错误。
  // 尝试MySQL后发现，一个字段上可以建立多个索引，这下方便了。
  {
    std::unordered_set<std::string> se(create_index.attrs.begin(), create_index.attrs.end());

    if (se.size() < create_index.attrs.size()) {
      return RC::VARIABLE_NOT_VALID;
    }

    for (auto &it : create_index.attrs) {
      if (table->table_meta().field(it.c_str()) == nullptr) {
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
    }
  }

  Index *index = table->find_index(create_index.index_name.c_str());
  if (nullptr != index) {
    LOG_WARN("index with name(%s) already exists. table name=%s", create_index.index_name.c_str(), table_name);
    return RC::SCHEMA_INDEX_NAME_REPEAT;
  }

  const FieldMeta *field_meta = table->table_meta().field(create_index.attrs.front().c_str());
  if (nullptr == field_meta) {
    LOG_WARN("no such field in table. db=%s, table=%s, field name=%s",
             db->name(), table_name, create_index.attrs.front().c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  stmt = new CreateIndexStmt(table, field_meta, create_index.index_name, create_index.is_unique);

  return RC::SUCCESS;
}
