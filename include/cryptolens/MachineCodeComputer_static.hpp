#pragma once

#include <string>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace MachineCodeComputer_static {}

} // namespace errors

/**
 */
class MachineCodeComputer_static
{
public:
  MachineCodeComputer_static(basic_Error & e) {}

  void
  set_machine_code(basic_Error & e, std::string machine_code);

  std::string
  get_machine_code(basic_Error & e);

private:
  std::string machine_code_;
};

} // namespace v20190401

namespace latest {

// TODO: errors namespace
using MachineCodeComputer_static = ::cryptolens_io::v20190401::MachineCodeComputer_static;

} // namespace latest

} // namespace cryptolens_io
