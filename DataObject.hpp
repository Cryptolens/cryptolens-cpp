#pragma once

namespace serialkeymanager_com {

class DataObject {
private:
  int id_;
  std::string name_;
  std::string string_value_;
  int int_value_;
public:
  // TOOD: Use ints with specific sizes...
  DataObject
    (int id
    , std::string name
    , std::string string_value
    , int int_value
    )
  : id_(id)
  , name_(std::move(name))
  , string_value_(std::move(string_value))
  , int_value_(int_value)
  {
    // TODO: Check requirements on max length of name and string_value
  }

  int                get_id() const;
  std::string const& get_name() const;
  std::string const& get_string_value() const;
  int                get_int_value() const;
};

} // namespace serialkeymanager_com
