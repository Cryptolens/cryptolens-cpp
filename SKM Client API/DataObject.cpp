#include "DataObject.hpp"

namespace serialkeymanager_com {

int
DataObject::get_id() const
{
  return id_;
}

std::string const&
DataObject::get_name() const
{
  return name_;
}

std::string const&
DataObject::get_string_value() const
{
  return string_value_;
}

int
DataObject::get_int_value() const
{
  return int_value_;
}

} // namespace serialkeymanager_com
