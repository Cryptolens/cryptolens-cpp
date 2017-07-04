#include <string>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "SignatureVerifier_OpenSSL.hpp"
#include "base64.hpp"
#include "optional.hpp"

namespace serialkeymanager_com {

void
verify(Error & e, RSA * rsa, std::string const& message, std::string const& sig)
{
  if (e) { return; }

  int r;

  if (rsa == NULL) { e.set(Subsystem::SignatureVerifier, 1); return; }

  EVP_MD_CTX * ctx = EVP_MD_CTX_create();
  if (ctx == NULL) { e.set(Subsystem::SignatureVerifier, 1); return; }

  EVP_PKEY * pkey = EVP_PKEY_new();
  if (pkey == NULL) { e.set(Subsystem::SignatureVerifier, 1); return; }

  r = EVP_PKEY_set1_RSA(pkey, rsa);
  if (r != 1) { e.set(Subsystem::SignatureVerifier, 1); return; }



  r = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey);
  if (r != 1) { e.set(Subsystem::SignatureVerifier, 1); return; }

  r = EVP_DigestVerifyUpdate(ctx, (unsigned char*)message.c_str(), message.size());
  if (r != 1) { e.set(Subsystem::SignatureVerifier, 1); return; }

  r = EVP_DigestVerifyFinal(ctx, (unsigned char*)sig.c_str(), sig.size());
  if (r != 1) { e.set(Subsystem::SignatureVerifier, 1); return; }

  // Void return type
  EVP_PKEY_free(pkey);

  // Void return type
  EVP_MD_CTX_destroy(ctx);
}

SignatureVerifier_OpenSSL::SignatureVerifier_OpenSSL()
{
  this->rsa = RSA_new();
  if (this->rsa != NULL) {
    this->rsa->n = BN_new();
    if (this->rsa->n == NULL) { RSA_free(this->rsa); }
  
    this->rsa->e = BN_new();
    if (this->rsa->e == NULL) { RSA_free(this->rsa); }
  }
}

SignatureVerifier_OpenSSL::~SignatureVerifier_OpenSSL()
{
  if (this->rsa != NULL) {
    RSA_free(this->rsa);
  }
}

void
SignatureVerifier_OpenSSL::set_modulus_base64(Error & e, std::string const& modulus_base64)
{
  this->set_modulus_base64_(e, modulus_base64);
  if (e) { e.set_call(Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

void
SignatureVerifier_OpenSSL::set_exponent_base64(Error & e, std::string const& exponent_base64)
{
  this->set_exponent_base64_(e, exponent_base64);
  if (e) { e.set_call(Call::SIGNATURE_VERIFIER_SET_EXPONENT_BASE64); }
}

void
SignatureVerifier_OpenSSL::set_modulus_base64_(Error & e, std::string const& modulus_base64)
{
  if (e) { return; }
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, 1); return; }

  optional<std::string> modulus = b64_decode(modulus_base64);
  if (!modulus) { e.set(Subsystem::SignatureVerifier, 1); return; }

  // FIXME: non-void return type
  BN_bin2bn((unsigned char*)modulus->c_str(),  modulus->size(),  this->rsa->n);
}

void
SignatureVerifier_OpenSSL::set_exponent_base64_(Error & e, std::string const& exponent_base64)
{
  if (e) { return; }
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, 1); return; }

  optional<std::string> exponent = b64_decode(exponent_base64);
  if (!exponent) { e.set(Subsystem::SignatureVerifier, 1); return; }

  // FIXME: non-void return type
  BN_bin2bn((unsigned char*)exponent->c_str(), exponent->size(), this->rsa->e);
}

bool
SignatureVerifier_OpenSSL::verify_message
  ( Error & e
  , std::string const& message
  , std::string const& signature_base64
  )
const
{
  if (e) { return false; }
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, 1); return false; }

  optional<std::string> sig = b64_decode(signature_base64);
  if (!sig) { e.set(Subsystem::SignatureVerifier, 1); return false; }

  verify(e, this->rsa, message, *sig);
  if (e) { return false; }

  return true;
}

} // namespace serialkeymanager_com
