#pragma once

#include <string>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace MachineCodeComputer_caching {}

} // namespace errors

/**
 */
template<typename MachineCodeComputer>
class MachineCodeComputer_caching
{
public:
  MachineCodeComputer_caching(basic_Error & e)
  : machine_code_()
  {
    MachineCodeComputer inner(e);
    machine_code_ = inner.get_machine_code(e);
  }

  std::string
  get_machine_code(basic_Error & e) {
    return machine_code_;
  }

private:
  std::string machine_code_;
};

} // namespace v20190401

namespace latest {

// TODO: errors namespace

template<typename MachineCodeComputer>
using MachineCodeComputer_caching = ::cryptolens_io::v20190401::MachineCodeComputer_caching<MachineCodeComputer>;

} // namespace latest

} // namespace cryptolens_io
