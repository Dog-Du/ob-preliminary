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
// Created by Longda on 2021/4/13.
//

#include <string.h>
#include <string>

#include "parse_stage.h"

#include "common/conf/ini.h"
#include "common/io/io.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "sql/parser/parse.h"
#include "sql/parser/value.h"

using namespace common;

RC ParseStage::handle_request(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;

  SqlResult         *sql_result = sql_event->session_event()->sql_result();
  const std::string &sql        = sql_event->sql();

  ParsedSqlResult parsed_sql_result;

  parse(sql.c_str(), &parsed_sql_result);
  if (parsed_sql_result.sql_nodes().empty()) {
    sql_result->set_return_code(RC::SUCCESS);
    sql_result->set_state_string("");
    return RC::INTERNAL;
  }

  if (parsed_sql_result.sql_nodes().size() > 1) {
    LOG_WARN("got multi sql commands but only 1 will be handled");
  }

  std::unique_ptr<ParsedSqlNode> sql_node = std::move(parsed_sql_result.sql_nodes().front());

  if (sql_node->flag == SCF_ERROR) {
    // set error information to event
    rc = RC::SQL_SYNTAX;
    sql_result->set_return_code(rc);
    sql_result->set_state_string("Failed to parse sql");
    return rc;
  }

  // 枚举所有可能出现 日期字面量 的地方。因为在正则表达式的地方已经进行了筛选，
  switch (sql_node->flag) {
    case SCF_INSERT: {
      for (auto &it : sql_node->insertion.values) {
        if (it.attr_type() == DATES && !check_date(it.get_date())) {
          rc = RC::VARIABLE_NOT_VALID;
          sql_result->set_return_code(rc);
          sql_result->set_state_string("Error date");
          return rc;
        }
      }
    } break;
    case SCF_UPDATE: {
      for (auto &it : sql_node->update.conditions) {
        if (it.left_value.attr_type() == DATES && it.left_is_attr == 0 && !check_date(it.left_value.get_date())) {
          rc = RC::VARIABLE_NOT_VALID;
          sql_result->set_return_code(rc);
          sql_result->set_state_string("Error date");
          return rc;
        }

        if (it.right_value.attr_type() == DATES && it.right_is_attr == 0 && !check_date(it.right_value.get_date())) {
          rc = RC::VARIABLE_NOT_VALID;
          sql_result->set_return_code(rc);
          sql_result->set_state_string("Error date");
          return rc;
        }
      }
    } break;
    default: break;
  }

  sql_event->set_sql_node(std::move(sql_node));

  return RC::SUCCESS;
}
