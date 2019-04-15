#pragma once

#include "RequestHandler_curl.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

namespace cryptolens_io {

namespace v20190401 {

struct Configuration_Unix {
  using RequestHandler = RequestHandler_curl;
  using SignatureVerifier = SignatureVerifier_OpenSSL;
};

} // namespace v20190401

namespace latest {

using Configuration_Unix = ::cryptolens_io::v20190401::Configuration_Unix;

} // namespace latest

} // namespace cryptolens_io
