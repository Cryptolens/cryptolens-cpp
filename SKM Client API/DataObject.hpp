#pragma once

namespace serialkeymanager_com {

// An immutable class representing an SKM Data Object
class DataObject {
private:
  int id_;
  std::string name_;
  std::string string_value_;
  int int_value_;
public:
  // TOOD: Use ints with specific sizes...
  DataObject
    ( int id
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

  // Returns the Id of the data object
  int                get_id() const;

  // Returns the name of the data object
  std::string const& get_name() const;

  // Returns the string value of the data object
  std::string const& get_string_value() const;

  // Returns the integer value of the data object
  int                get_int_value() const;
};

} // namespace serialkeymanager_com
