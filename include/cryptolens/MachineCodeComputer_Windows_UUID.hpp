#pragma once

#include <string>

#include "basic_Error.hpp"
#include "MachineCodeComputer_caching.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace MachineCodeComputer_Windows_UUID_Recompute {}

} // namespace errors

/**
 * Computes a unique machine code for the device using the COM library
 * available on Windows.
 *
 * This class recomputes the machine code each time get_machine_code()
 * is called.
 *
 * Requires the project to link against Iphlpapi.lib
 */
class MachineCodeComputer_Windows_UUID_Recompute
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
	  MachineCodeComputer_Windows_UUID_Recompute(basic_Error & e) {}

  std::string
  get_machine_code(basic_Error & e);

private:
  std::string machine_code_;
};

/**
 * Computes a unique machine code for the device using the COM library
 * available on Windows.
 *
 * This class computes the machine code when created, and the same machine
 * code is returned on subsequent calls to get_machine_code(). This
 * can be convenient when used with floating licensing since
 * we do not want the machine code to change while the program is running.
 *
 * Requires the project to link against Iphlpapi.lib
 */
using MachineCodeComputer_Windows_UUID = MachineCodeComputer_caching<MachineCodeComputer_Windows_UUID_Recompute>;

} // namespace v20190401

namespace latest {

namespace errors {

namespace MachineCodeComputer_Windows_UUID_Recompute = ::cryptolens_io::v20190401::errors::MachineCodeComputer_Windows_UUID_Recompute;

} // namespace errors

/**
 * Computes a unique machine code for the device using the COM library
 * available on Windows.
 *
 * This class recomputes the machine code each time get_machine_code()
 * is called.
 *
 * Requires the project to link against Iphlpapi.lib
 */
using MachineCodeComputer_Windows_UUID_Recompute = ::cryptolens_io::v20190401::MachineCodeComputer_Windows_UUID_Recompute;

/**
 * Computes a unique machine code for the device using the COM library
 * available on Windows.
 *
 * This class computes the machine code when created, and the same machine
 * code is returned on subsequent calls to get_machine_code(). This
 * can be convenient when used with floating licensing since
 * we do not want the machine code to change while the program is running.
 *
 * Requires the project to link against Iphlpapi.lib
 */
using MachineCodeComputer_Windows_UUID = ::cryptolens_io::v20190401::MachineCodeComputer_Windows_UUID;

} // namespace latest

} // namespace cryptolens_io
