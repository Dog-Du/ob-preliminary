/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its
affiliates. All rights reserved. miniob is licensed under Mulan PSL v2. You can
use this software according to the terms and conditions of the Mulan PSL v2. You
may obtain a copy of Mulan PSL v2 at: http://license.coscl.org.cn/MulanPSL2 THIS
SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Wangyunlai on 2021/5/13.
//

#include "storage/table/view.h"
#include <cstdio>
#include <limits.h>
#include <string.h>

#include "common/defs.h"
#include "common/lang/string.h"
#include "common/lang/span.h"
#include "common/lang/algorithm.h"
#include "common/log/log.h"
#include "common/global_context.h"
#include "common/rc.h"
#include "storage/db/db.h"
#include "storage/buffer/disk_buffer_pool.h"
#include "storage/common/condition_filter.h"
#include "storage/common/meta_util.h"
#include "storage/field/field_meta.h"
#include "storage/field/field.h"
#include "storage/index/bplus_tree_index.h"
#include "storage/index/index.h"
#include "storage/record/record.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

RC View::create(Db *db, int32_t table_id, const char *path, const char *name, const char *base_dir,
    span<const AttrInfoSqlNode> attr_infos, const std::vector<Field> &fields, const SelectSqlNode &select_sql,
    StorageFormat storage_format, bool can_write)
{
  if (table_id < 0 || common::is_blank(name) || attr_infos.empty()) {
    LOG_WARN("invalid argument.");
    return RC::INVALID_ARGUMENT;
  }

  ASSERT(fields.size() == attr_infos.size(), "");
  can_write_  = can_write;
  fields_     = fields;
  select_sql_ = select_sql;
  db_         = db;
  view_name_  = name;
  table_id_   = table_id;

  RC rc = table_meta_.init(table_id, name, db->trx_kit().trx_fields(), attr_infos, storage_format);
  if (rc != RC::SUCCESS) {
    LOG_WARN("table_meta init failed.");
    return rc;
  }

  if (can_write) {
    table_ = const_cast<Table *>(fields.front().table());  // 如果是可写的，
    ASSERT(table_!=nullptr,"");
  }

  return RC::SUCCESS;
}