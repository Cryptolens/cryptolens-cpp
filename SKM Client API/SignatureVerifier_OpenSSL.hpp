#pragma once

#include <string>

#include <openssl/rsa.h>

#include "Error.hpp"

namespace serialkeymanager_com {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Serialkeymanager.com Web API. This signature
 * verifier can be used with the OpenSSL or LibreSSL libraries.
 *
 * The signatures are used to make sure the replies from the
 * Serialkeymanager.com Web API have not been modified.
 */
class SignatureVerifier_OpenSSL
{
public:
  SignatureVerifier_OpenSSL();

  ~SignatureVerifier_OpenSSL();

  // Set the modulus, found under Security Settings on serialkeymanager.com
  void set_modulus_base64(Error & e, std::string const& modulus_base64);

  // Set the exponent, found under Security Settings on serialkeymanager.com
  void set_exponent_base64(Error & e, std::string const& exponent_base64);

  // Verifies that the message is correct using the provided signature
  bool verify_message(Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  RSA * rsa;

  void set_modulus_base64_(Error & e, std::string const& modulus_base64);
  void set_exponent_base64_(Error & e, std::string const& exponent_base64);
};

} // namespace serialkeymanager_com
