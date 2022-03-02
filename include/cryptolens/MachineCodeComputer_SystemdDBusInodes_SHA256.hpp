#pragma once

#include <string>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace MachineCodeComputer_SystemdDBusInodes_SHA256 {}

} // namespace errors

/**
 */
class MachineCodeComputer_SystemdDBusInodes_SHA256
{
public:
  explicit
  MachineCodeComputer_SystemdDBusInodes_SHA256(basic_Error & e) {}

  std::string
  get_machine_code(basic_Error & e);
};

} // namespace v20190401

namespace latest {

// TODO: errors namespace
using MachineCodeComputer_SystemdDBusInodes_SHA256 = ::cryptolens_io::v20190401::MachineCodeComputer_SystemdDBusInodes_SHA256;

} // namespace latest

} // namespace cryptolens_io
