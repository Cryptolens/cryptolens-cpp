#pragma once

#include <string>

#include <openssl/rsa.h>

namespace serialkeymanager_com {

class SignatureVerifier_OpenSSL
{
public:
  SignatureVerifier_OpenSSL();

  ~SignatureVerifier_OpenSSL();

  void set_modulus_base64(std::string const& modulus_base64);

  void set_exponent_base64(std::string const& exponent_base64);

  bool verify_message(std::string const& message, std::string const& signature_base64) const;

private:
  RSA * rsa;
};

} // namespace serialkeymanager_com
