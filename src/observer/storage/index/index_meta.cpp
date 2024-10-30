/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"
#include <cstddef>
#include <json/value.h>
#include <sstream>

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");
const static Json::StaticString FIELD_NUM("field_num");
const static Json::StaticString FIELD_UNIQUE("unique");

RC IndexMeta::init(const char            *name,
    const std::vector<const FieldMeta *> &fields_meta, bool unique)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  for (auto &field : fields_meta) {
    if (common::is_blank(field->name())) {
      LOG_ERROR("Failed to init index, name is empty.");
      return RC::INVALID_ARGUMENT;
    }
    fields_.push_back(std::string(field->name()));
  }
  name_   = name;
  unique_ = unique;
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME]   = name_;
  json_value[FIELD_UNIQUE] = unique_;
  json_value[FIELD_NUM]    = fields_.size();

  Json::Value fields_names;
  for (int i = 0; i < fields_.size(); i++) {
    fields_names[i] = fields_[i];
  }

  json_value[FIELD_FIELD_NAME] = std::move(fields_names);
}

RC IndexMeta::from_json(
    const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &field_name_value = json_value[FIELD_NAME];
  const Json::Value &field_unique     = json_value[FIELD_UNIQUE];
  const Json::Value &field_num        = json_value[FIELD_NUM];
  const Json::Value &field_value      = json_value[FIELD_FIELD_NAME];

  if (!field_name_value.isString()) {
    LOG_ERROR("Index name error. json value=%s", field_name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!field_unique.isBool()) {
    LOG_ERROR("Index unique erro. json value=%s", field_unique.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!field_num.isInt()) {
    LOG_ERROR("Field num error. json value=%s",
        field_name_value.asCString(),
        field_num.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (field_num.asInt() != field_value.size()) {
    LOG_ERROR("Field num error not equal field size.");
    return RC::INTERNAL;
  }

  for (int i = 0; i < field_num.asInt(); i++) {
    if (!field_value[i].isString()) {
      LOG_ERROR("Field name of index [%s] is not a string. json value=%s",
          field_name_value.asCString(),
          field_value.toStyledString().c_str());
      return RC::INTERNAL;
    }
  }

  std::vector<const FieldMeta *> fields;
  for (int i = 0; i < field_value.size(); i++) {
    const FieldMeta *field = table.field(field_value[i].asCString());
    if (nullptr == field) {
      LOG_ERROR("no such field");
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields.emplace_back(field);
  }

  return index.init(
      field_name_value.asCString(), fields, field_unique.asBool());
}

const char *IndexMeta::name() const { return name_.c_str(); }

const std::vector<std::string> &IndexMeta::fields() const { return fields_; }

void IndexMeta::desc(ostream &os) const
{
  std::stringstream output;

  for (auto &field : fields_) {
    output << field << " ";
  }

  os << "index name=" << name_ << ", fields=" << output.str();
}