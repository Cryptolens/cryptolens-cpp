#pragma once

#include "ResponseParser_ArduinoJson5.hpp"
#include "RequestHandler_WinHTTP.hpp"
#include "SignatureVerifier_CryptoAPI.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename MachineCodeComputer_>
struct Configuration_Windows {
  using ResponseParser = ResponseParser_ArduinoJson5;
  using RequestHandler = RequestHandler_WinHTTP;
  using SignatureVerifier = SignatureVerifier_CryptoAPI;
  using MachineCodeComputer = MachineCodeComputer_;
};

} // namespace v20190401

namespace latest {

template<typename MachineCodeComputer_>
using Configuration_Windows = ::cryptolens_io::v20190401::Configuration_Windows<MachineCodeComputer_>;

} // namespace latest

} // namespace cryptolens_io
