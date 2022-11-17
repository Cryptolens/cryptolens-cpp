#pragma once

#include <string>

#include <bearssl_rsa.h>

#include "basic_Error.hpp"
#include "Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier can be used with the OpenSSL or LibreSSL libraries.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
class SignatureVerifier_BearSSL
{
public:
  explicit
  SignatureVerifier_BearSSL(basic_Error & e);

  SignatureVerifier_BearSSL(SignatureVerifier_BearSSL const&) = delete;
  SignatureVerifier_BearSSL(SignatureVerifier_BearSSL &&) = delete;
  void operator=(SignatureVerifier_BearSSL const&) = delete;
  void operator=(SignatureVerifier_BearSSL &&) = delete;
  ~SignatureVerifier_BearSSL();

  void set_modulus_base64(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(basic_Error & e, std::string const& exponent_base64);

  bool verify_message(basic_Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  br_rsa_public_key pk_;

  void set_modulus_base64_(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64_(basic_Error & e, std::string const& exponent_base64);
};

} // namespace v20190401

namespace latest {

using SignatureVerifier_BearSSL = ::cryptolens_io::v20190401::SignatureVerifier_BearSSL;

} // namespace latest

} // namespace cryptolens_io
