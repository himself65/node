#include "json_parser.h"
#include "util.h"

namespace node {

JSONParser::JSONParser() {}

bool JSONParser::Parse(const std::string& content) {
  DCHECK(!parsed_);

  content_string_ = content;
  content_string_.append(simdjson::SIMDJSON_PADDING, ' ');

  document_ = std::make_unique<simdjson::ondemand::document>();

  simdjson::error_code error = json_parser_.iterate(content_string_).get(*document_);
  if (error) {
    return false;
  }

  simdjson::ondemand::object main_object;
  error = document_->get_object().get(main_object);
  if (error) {
    return false;
  }

  parsed_ = true;
  return true;
}

std::optional<std::string> JSONParser::GetTopLevelStringField(
    std::string_view field) {
  if (!parsed_ || !document_) {
    return {};
  }

  simdjson::ondemand::object main_object;
  simdjson::error_code error = document_->get_object().get(main_object);
  if (error) {
    return {};
  }

  simdjson::ondemand::value value;
  error = main_object[field].get(value);
  if (error) {
    return {};
  }

  std::string_view string_value;
  error = value.get_string().get(string_value);
  if (error) {
    return {};
  }

  return std::string(string_value);
}

std::optional<bool> JSONParser::GetTopLevelBoolField(std::string_view field) {
  if (!parsed_ || !document_) {
    return {};
  }

  simdjson::ondemand::object main_object;
  simdjson::error_code error = document_->get_object().get(main_object);
  if (error) {
    return {};
  }

  simdjson::ondemand::value value;
  error = main_object[field].get(value);
  if (error) {
    if (error == simdjson::error_code::NO_SUCH_FIELD) {
      return false;
    }
    return {};
  }

  bool bool_value;
  error = value.get_bool().get(bool_value);
  if (error) {
    return {};
  }

  return bool_value;
}

std::optional<JSONParser::StringDict> JSONParser::GetTopLevelStringDict(
    std::string_view field) {
  if (!parsed_ || !document_) {
    return std::nullopt;
  }

  simdjson::ondemand::object main_object;
  simdjson::error_code error = document_->get_object().get(main_object);
  if (error) {
    return std::nullopt;
  }

  simdjson::ondemand::value value;
  error = main_object[field].get(value);
  if (error) {
    if (error == simdjson::error_code::NO_SUCH_FIELD) {
      return StringDict();
    }
    return std::nullopt;
  }

  simdjson::ondemand::object dict_object;
  error = value.get_object().get(dict_object);
  if (error) {
    return std::nullopt;
  }

  std::unordered_map<std::string, std::string> result;
  for (auto field_pair : dict_object) {
    simdjson::ondemand::raw_json_string key;
    simdjson::ondemand::value dict_value;

    error = field_pair.key().get(key);
    if (error) {
      return StringDict();
    }

    error = field_pair.value().get(dict_value);
    if (error) {
      return StringDict();
    }

    std::string_view string_value;
    error = dict_value.get_string().get(string_value);
    if (error) {
      return StringDict();
    }

    result.emplace(std::string(key.raw()), std::string(string_value));
  }
  return result;
}

}  // namespace node
