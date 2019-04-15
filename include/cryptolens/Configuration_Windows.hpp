#pragma once

#include "RequestHandler_WinHTTP.hpp"
#include "SignatureVerifier_CryptoAPI.hpp"

namespace cryptolens_io {

namespace v20190401 {

struct Configuration_Windows {
  using RequestHandler = RequestHandler_WinHTTP;
  using SignatureVerifier = SignatureVerifier_CryptoAPI;
};

} // namespace v20190401

namespace latest {

using Configuration_Windows = ::cryptolens_io::v20190401::Configuration_Windows;

} // namespace latest

} // namespace cryptolens_io
