#include <string>

#include "imports/std/optional"

#include "imports/openssl/bn.h"
#include "imports/openssl/evp.h"
#include "imports/openssl/rsa.h"

#include "api.hpp"
#include "base64.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

namespace {

constexpr int RSA_NULL = 1;
constexpr int CTX_CREATE_FAILED = 2;
constexpr int PKEY_NEW_FAILED = 3;
constexpr int PKEY_SET1_RSA_FAILED = 4;
constexpr int DIGEST_VERIFY_INIT_FAILED = 5;
constexpr int DIGEST_VERIFY_UPDATE_FAILED = 6;
constexpr int DIGEST_VERIFY_FINAL_FAILED = 7;
constexpr int BN_BIN2BN_FAILED = 8;
constexpr int BN_NEW_FAILED = 9;
constexpr int RSA_SET0_KEY_FAILED = 10;

} // namespace

namespace cryptolens_io {

namespace v20190401 {

void
verify(basic_Error & e, RSA * rsa, std::vector<unsigned char> const& message, std::vector<unsigned char> const& sig)
{
  using namespace errors;
  api::main api;

  if (e) { return; }

  int r;
  EVP_MD_CTX * ctx = NULL;
  EVP_PKEY * pkey = NULL;

  if (rsa == NULL) { e.set(api, Subsystem::SignatureVerifier, RSA_NULL); goto end; }

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  ctx = EVP_MD_CTX_create();
#else
  ctx = EVP_MD_CTX_new();
#endif
  if (ctx == NULL) { e.set(api, Subsystem::SignatureVerifier, CTX_CREATE_FAILED); goto end; }

  pkey = EVP_PKEY_new();
  if (pkey == NULL) { e.set(api, Subsystem::SignatureVerifier, PKEY_NEW_FAILED); goto end; }

  r = EVP_PKEY_set1_RSA(pkey, rsa);
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, PKEY_SET1_RSA_FAILED); goto end; }



  r = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey);
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_INIT_FAILED); goto end; }

  r = EVP_DigestVerifyUpdate(ctx, (unsigned char*)message.data(), message.size());
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_UPDATE_FAILED); goto end; }

  r = EVP_DigestVerifyFinal(ctx, (unsigned char*)sig.data(), sig.size());
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_FINAL_FAILED); goto end; }

end:
  // Void return type
  EVP_PKEY_free(pkey);

  // Void return type
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  EVP_MD_CTX_destroy(ctx);
#else
  EVP_MD_CTX_free(ctx);
#endif
}

SignatureVerifier_OpenSSL::SignatureVerifier_OpenSSL(basic_Error & e)
{
  this->rsa = RSA_new();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  if (this->rsa != NULL) {
    this->rsa->n = BN_new();
    if (this->rsa->n == NULL) { RSA_free(this->rsa); return; }

    this->rsa->e = BN_new();
    if (this->rsa->e == NULL) { RSA_free(this->rsa); return; }
  }
#else
  if (this->rsa != NULL) {
    BIGNUM *n, *e;
    n = BN_new();
    e = BN_new();

    if (n == NULL || e == NULL) {
      RSA_free(this->rsa);
      this->rsa = NULL;

      if (n != NULL) { BN_free(n); }
      if (e != NULL) { BN_free(e); }
    } else {
      int result = RSA_set0_key(this->rsa, n, e, NULL);
      if (result != 1) { BN_free(n); BN_free(e); RSA_free(this->rsa); this->rsa = NULL; }
    }
  }

#endif
}

SignatureVerifier_OpenSSL::~SignatureVerifier_OpenSSL()
{
  if (this->rsa != NULL) {
    RSA_free(this->rsa);
  }
}

/**
 * Sets the modulus of the public key used by the cryptolens.io Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on cryptolens.io at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "AbC=" should be passed to this method.
 */
void
SignatureVerifier_OpenSSL::set_modulus_base64(basic_Error & e, std::string const& modulus_base64)
{
  if (e) { return; }
  this->set_modulus_base64_(e, modulus_base64);
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

/**
 * Sets the exponent of the public key used by the cryptolens.io Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on cryptolens.io at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "deFG" should be passed to this method.
 */
void
SignatureVerifier_OpenSSL::set_exponent_base64(basic_Error & e, std::string const& exponent_base64)
{
  if (e) { return; }
  this->set_exponent_base64_(e, exponent_base64);
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_EXPONENT_BASE64); }
}

void
SignatureVerifier_OpenSSL::set_modulus_base64_(basic_Error & e, std::string const& modulus_base64)
{
  if (e) { return; }
  if (this->rsa == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_NULL); return; }

  optional<std::vector<unsigned char>> modulus = ::cryptolens_io::v20190401::internal::b64_decode(modulus_base64);
  if (!modulus) { e.set(api::main(), errors::Subsystem::Base64); return; }

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  BIGNUM * n = BN_bin2bn((unsigned char*)modulus->data(),  modulus->size(),  this->rsa->n);
  if (n == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_BIN2BN_FAILED); return; }
#else
  BIGNUM const* exp_current;

  // void return type
  RSA_get0_key(this->rsa, NULL, &exp_current, NULL);

  BIGNUM * n = BN_bin2bn((unsigned char*)modulus->data(),  modulus->size(), NULL);
  if (n == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_BIN2BN_FAILED); return; }

  BIGNUM * exp;
  if (exp_current == NULL) {
    // Requirements for RSA_set0_key() below is that the first time we call it, both n and e must
    // be set. So in case e has not been set we allocate a dummy BIGNUM here and use that.
    exp = BN_new();
    if (exp == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_NEW_FAILED); BN_free(n); return; }
  } else {
    // If e is already set it is owned by this->rsa, and it is not a valid argument to RSA_set0_key()
    exp = NULL;
  }

  int result = RSA_set0_key(this->rsa, n, exp, NULL);
  if (result != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_SET0_KEY_FAILED); BN_free(n); BN_free(exp); return; }
#endif
}

void
SignatureVerifier_OpenSSL::set_exponent_base64_(basic_Error & e, std::string const& exponent_base64)
{
  if (e) { return; }
  if (this->rsa == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_NULL); return; }

  optional<std::vector<unsigned char>> exponent = ::cryptolens_io::v20190401::internal::b64_decode(exponent_base64);
  if (!exponent) { e.set(api::main(), errors::Subsystem::Base64); return; }

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  BIGNUM * exp = BN_bin2bn(exponent->data(), exponent->size(), this->rsa->e);
  if (exp == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_BIN2BN_FAILED); return; }
#else
  BIGNUM const* n_current;

  // void return type
  RSA_get0_key(this->rsa, &n_current, NULL, NULL);

  BIGNUM * exp = BN_bin2bn((unsigned char*)exponent->data(),  exponent->size(), NULL);
  if (exp == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_NEW_FAILED); return; }

  BIGNUM * n;
  if (n_current == NULL) {
    // Requirements for RSA_set0_key() below is that the first time we call it, both n and e must
    // be set. So in case n has not been set we allocate a dummy BIGNUM here and use that.
    n = BN_new();
    if (n == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, BN_NEW_FAILED); BN_free(exp); return; }
  } else {
    // If n is already set it is owned by this->rsa, and it is not a valid argument to RSA_set0_key()
    n = NULL;
  }

  int result = RSA_set0_key(this->rsa, n, exp, NULL);
  if (result != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_SET0_KEY_FAILED); BN_free(n); BN_free(exp); return; }
#endif
}

/**
 * This function is used internally by the library and need not be called.
 */
bool
SignatureVerifier_OpenSSL::verify_message
  ( basic_Error & e
  , std::vector<unsigned char> const& message
  , std::string const& signature_base64
  )
const
{
  if (e) { return false; }
  if (this->rsa == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_NULL); return false; }

  optional<std::vector<unsigned char>> sig = ::cryptolens_io::v20190401::internal::b64_decode(signature_base64);
  if (!sig) { e.set(api::main(), errors::Subsystem::Base64); return false; }

  verify(e, this->rsa, message, *sig);
  if (e) { return false; }

  return true;
}

} // namespace v20190401

} // namespace cryptolens_io
