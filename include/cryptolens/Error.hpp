#pragma once

#include "basic_Error.hpp"

namespace serialkeymanager_com {

class Error : public basic_Error {
public:
  // Declarations below hide these methods inherited from the basic_Error class
  using basic_Error::get_subsystem;
  using basic_Error::get_reason;
  using basic_Error::get_extra;

  int get_subsystem() const noexcept { return get_subsystem(api::main()); }
  int get_reason() const noexcept { return get_reason(api::main()); }
  size_t get_extra() const noexcept { return get_extra(api::main()); }
};

} // namespace serialkeymanager_com
