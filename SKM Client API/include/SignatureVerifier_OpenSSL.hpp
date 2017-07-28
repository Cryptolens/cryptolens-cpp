#pragma once

#include <string>

#include <openssl/rsa.h>

#include "Error.hpp"

namespace serialkeymanager_com {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Serialkeymanager.com Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier can be used with the OpenSSL or LibreSSL libraries.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
class SignatureVerifier_OpenSSL
{
public:
  SignatureVerifier_OpenSSL();

  ~SignatureVerifier_OpenSSL();

  void set_modulus_base64(Error & e, std::string const& modulus_base64);
  void set_exponent_base64(Error & e, std::string const& exponent_base64);

  bool verify_message(Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  RSA * rsa;

  void set_modulus_base64_(Error & e, std::string const& modulus_base64);
  void set_exponent_base64_(Error & e, std::string const& exponent_base64);
};

} // namespace serialkeymanager_com
