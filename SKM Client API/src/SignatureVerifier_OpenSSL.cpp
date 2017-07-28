#include <string>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "SignatureVerifier_OpenSSL.hpp"
#include "base64.hpp"
#include "optional.hpp"

namespace serialkeymanager_com {

namespace {

constexpr int RSA_NULL = 1;
constexpr int CTX_CREATE_FAILED = 2;
constexpr int PKEY_NEW_FAILED = 3;
constexpr int PKEY_SET1_RSA_FAILED = 4;
constexpr int DIGEST_VERIFY_INIT_FAILED = 5;
constexpr int DIGEST_VERIFY_UPDATE_FAILED = 6;
constexpr int DIGEST_VERIFY_FINAL_FAILED = 7;

}

void
verify(Error & e, RSA * rsa, std::string const& message, std::string const& sig)
{
  if (e) { return; }

  int r;

  if (rsa == NULL) { e.set(Subsystem::SignatureVerifier, RSA_NULL); return; }

  EVP_MD_CTX * ctx = EVP_MD_CTX_create();
  if (ctx == NULL) { e.set(Subsystem::SignatureVerifier, CTX_CREATE_FAILED); return; }

  EVP_PKEY * pkey = EVP_PKEY_new();
  if (pkey == NULL) { e.set(Subsystem::SignatureVerifier, PKEY_NEW_FAILED); return; }

  r = EVP_PKEY_set1_RSA(pkey, rsa);
  if (r != 1) { e.set(Subsystem::SignatureVerifier, PKEY_SET1_RSA_FAILED); return; }



  r = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey);
  if (r != 1) { e.set(Subsystem::SignatureVerifier, DIGEST_VERIFY_INIT_FAILED); return; }

  r = EVP_DigestVerifyUpdate(ctx, (unsigned char*)message.c_str(), message.size());
  if (r != 1) { e.set(Subsystem::SignatureVerifier, DIGEST_VERIFY_UPDATE_FAILED); return; }

  r = EVP_DigestVerifyFinal(ctx, (unsigned char*)sig.c_str(), sig.size());
  if (r != 1) { e.set(Subsystem::SignatureVerifier, DIGEST_VERIFY_FINAL_FAILED); return; }

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

/**
 * Sets the modulus of the public key used by the Serialkeymanager.com Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on serialkeymanager.com at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "AbC=" should be passed to this method.
 */
void
SignatureVerifier_OpenSSL::set_modulus_base64(Error & e, std::string const& modulus_base64)
{
  this->set_modulus_base64_(e, modulus_base64);
  if (e) { e.set_call(Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

/**
 * Sets the exponent of the public key used by the Serialkeymanager.com Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on serialkeymanager.com at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "deFG" should be passed to this method.
 */
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
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, RSA_NULL); return; }

  optional<std::string> modulus = b64_decode(modulus_base64);
  if (!modulus) { e.set(Subsystem::Base64); return; }

  // FIXME: non-void return type
  BN_bin2bn((unsigned char*)modulus->c_str(),  modulus->size(),  this->rsa->n);
}

void
SignatureVerifier_OpenSSL::set_exponent_base64_(Error & e, std::string const& exponent_base64)
{
  if (e) { return; }
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, RSA_NULL); return; }

  optional<std::string> exponent = b64_decode(exponent_base64);
  if (!exponent) { e.set(Subsystem::Base64); return; }

  // FIXME: non-void return type
  BN_bin2bn((unsigned char*)exponent->c_str(), exponent->size(), this->rsa->e);
}

/**
 * This function is used internally by the library and need not be called.
 */
bool
SignatureVerifier_OpenSSL::verify_message
  ( Error & e
  , std::string const& message
  , std::string const& signature_base64
  )
const
{
  if (e) { return false; }
  if (this->rsa == NULL) { e.set(Subsystem::SignatureVerifier, RSA_NULL); return false; }

  optional<std::string> sig = b64_decode(signature_base64);
  if (!sig) { e.set(Subsystem::Base64); return false; }

  verify(e, this->rsa, message, *sig);
  if (e) { return false; }

  return true;
}

} // namespace serialkeymanager_com
