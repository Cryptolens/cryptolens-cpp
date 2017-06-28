#include <string>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "SignatureVerifier_OpenSSL.hpp"
#include "base64.hpp"
#include "optional.hpp"

namespace serialkeymanager_com {

int
verify(Error & e, RSA * rsa, std::string const& message, std::string const& sig)
{
  int r;

  if (rsa == NULL) { return 0; }

  EVP_MD_CTX * ctx = EVP_MD_CTX_create();
  if (ctx == NULL) { return 0; }

  EVP_PKEY * pkey = EVP_PKEY_new();
  if (pkey == NULL) { return 0; }

  r = EVP_PKEY_set1_RSA(pkey, rsa);
  if (r != 1) { return 0; }



  r = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey);
  if (r != 1) { return 0; }

  r = EVP_DigestVerifyUpdate(ctx, (unsigned char*)message.c_str(), message.size());
  if (r != 1) { return 0; }

  r = EVP_DigestVerifyFinal(ctx, (unsigned char*)sig.c_str(), sig.size());

  // Void return type
  EVP_PKEY_free(pkey);

  // Void return type
  EVP_MD_CTX_destroy(ctx);

  return r;
}

SignatureVerifier_OpenSSL::SignatureVerifier_OpenSSL()
{
  // This should be redesigned. We dont want to throw exceptions
  // from the constructor, so for now if initialization fails we
  // just make sure the class never accepts a signature
 
  this->rsa = RSA_new();
  //if (this->rsa == NULL) { }

  this->rsa->n = BN_new();
  if (this->rsa->n == NULL) { RSA_free(this->rsa); }

  this->rsa->e = BN_new();
  if (this->rsa->e == NULL) { RSA_free(this->rsa); }
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
  if (this->rsa == NULL) { return; }

  optional<std::string> modulus = b64_decode(modulus_base64);

  if (!modulus) {
    // TODO: This just silently failes
    return;
  }

  BN_bin2bn((unsigned char*)modulus->c_str(),  modulus->size(),  this->rsa->n);
}

void
SignatureVerifier_OpenSSL::set_exponent_base64(Error & e, std::string const& exponent_base64)
{
  if (this->rsa == NULL) { return; }

  optional<std::string> exponent = b64_decode(exponent_base64);

  if (!exponent) {
    // TODO: This just silently failes
    return;
  }

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
  if (this->rsa == NULL) { return false; }

  optional<std::string> sig = b64_decode(signature_base64);

  if (!sig || verify(e, this->rsa, message, *sig) != 1) {
    return false;
  } else {
    return true;
  }
}

} // namespace serialkeymanager_com
