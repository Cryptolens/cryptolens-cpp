#include "MachineCodeComputer_static.hpp"

namespace cryptolens_io {

namespace v20190401 {

void
MachineCodeComputer_static::set_machine_code(basic_Error & e, std::string machine_code)
{
  if (e) { return; }

  this->machine_code_ = std::move(machine_code);
}

std::string
MachineCodeComputer_static::get_machine_code(basic_Error & e)
{
  return this->machine_code_;
}

} // namespace v20190401

} // namespace cryptolens_io
