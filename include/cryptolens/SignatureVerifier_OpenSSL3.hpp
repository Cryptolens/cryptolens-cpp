#pragma once

#include <string>

#include <openssl/evp.h>
#include <openssl/params.h>
#include <openssl/param_build.h>

#include "basic_Error.hpp"
#include "Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier can be used with the OpenSSL versions starting from 3.0.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_public_key_base64() method.
 */
class SignatureVerifier_OpenSSL3
{
public:
  explicit
  SignatureVerifier_OpenSSL3(basic_Error & e);
  SignatureVerifier_OpenSSL3(SignatureVerifier_OpenSSL3 const&) = delete;
  SignatureVerifier_OpenSSL3(SignatureVerifier_OpenSSL3 &&) = delete;
  void operator=(SignatureVerifier_OpenSSL3 const&) = delete;
  void operator=(SignatureVerifier_OpenSSL3 &&) = delete;
  ~SignatureVerifier_OpenSSL3();

  void set_public_key_base64(basic_Error & e, std::string const& key);
  void set_public_key_base64(basic_Error & e, std::string const& modulus_base64, std::string const& exponent_base64);

  void set_modulus_base64(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(basic_Error & e, std::string const& exponent_base64);

  bool verify_message(basic_Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  EVP_PKEY *pkey_;

  void set_public_key_base64_(basic_Error & e, std::string const& modulus_base64, std::string const& exponent_base64);
};

} // namespace v20190401

namespace latest {

using SignatureVerifier_OpenSSL3 = ::cryptolens_io::v20190401::SignatureVerifier_OpenSSL3;

} // namespace latest

} // namespace cryptolens_io
