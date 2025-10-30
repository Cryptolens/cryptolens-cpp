#pragma once

#include "ResponseParser_ArduinoJson7.hpp"
#include "RequestHandler_curl.hpp"

#include <openssl/opensslv.h>
#if OPENSSL_VERSION_NUMBER >= 0x030000000
#include "SignatureVerifier_OpenSSL3.hpp"
#else
#include "SignatureVerifier_OpenSSL.hpp"
#endif

#include "validators/AndValidator.hpp"
#include "validators/CorrectKeyValidator.hpp"
#include "validators/CorrectProductValidator.hpp"
#include "validators/NotExpiredValidator_ctime.hpp"
#include "validators/OnValidMachineValidator.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename MachineCodeComputer_>
struct Configuration_Unix {
  using ResponseParser = ResponseParser_ArduinoJson7;
  using RequestHandler = RequestHandler_curl;
  using MachineCodeComputer = MachineCodeComputer_;

#if OPENSSL_VERSION_NUMBER >= 0x030000000
  using SignatureVerifier = SignatureVerifier_OpenSSL3;
#else
  using SignatureVerifier = SignatureVerifier_OpenSSL;
#endif

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                          , AndValidator_<Env, CorrectProductValidator_<Env>
                          , AndValidator_<Env, OnValidMachineValidator_<Env>
                          ,                    NotExpiredValidator_ctime_<Env>
                          >>>;

  template<typename Env>
  using GetKeyValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                        , AndValidator_<Env, CorrectProductValidator_<Env>
                        >>;
};

template<typename MachineCodeComputer_>
struct Configuration_Unix_IgnoreExpires {
  using ResponseParser = ResponseParser_ArduinoJson7;
  using RequestHandler = RequestHandler_curl;
  using MachineCodeComputer = MachineCodeComputer_;

#if OPENSSL_VERSION_NUMBER >= 0x030000000
  using SignatureVerifier = SignatureVerifier_OpenSSL3;
#else
  using SignatureVerifier = SignatureVerifier_OpenSSL;
#endif

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                          , AndValidator_<Env, CorrectProductValidator_<Env>
                          ,                    OnValidMachineValidator_<Env>
                          >>;

  template<typename Env>
  using GetKeyValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                        , AndValidator_<Env, CorrectProductValidator_<Env>
                        >>;
};

template<typename MachineCodeComputer_>
using Configuration_Unix_CheckExpires = ::cryptolens_io::v20190401::Configuration_Unix<MachineCodeComputer_>;

} // namespace v20190401

namespace latest {

template<typename MachineCodeComputer_>
using Configuration_Unix = ::cryptolens_io::v20190401::Configuration_Unix<MachineCodeComputer_>;

template<typename MachineCodeComputer_>
using Configuration_Unix_CheckExpires = ::cryptolens_io::v20190401::Configuration_Unix_CheckExpires<MachineCodeComputer_>;

template<typename MachineCodeComputer_>
using Configuration_Unix_IgnoreExpires = ::cryptolens_io::v20190401::Configuration_Unix_IgnoreExpires<MachineCodeComputer_>;

} // namespace latest

} // namespace cryptolens_io
