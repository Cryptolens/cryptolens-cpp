#include <string>

#include "imports/std/optional"

#include "imports/openssl/bn.h"
#include "imports/openssl/evp.h"
#include "imports/openssl/rsa.h"

#include "api.hpp"
#include "base64.hpp"
#include "SignatureVerifier_OpenSSL3.hpp"

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

namespace {

void
verify(basic_Error & e, EVP_PKEY * pkey, std::string const& message, std::string const& sig)
{
  using namespace errors;
  api::main api;

  if (e) { return; }

  int r;
  EVP_MD_CTX * ctx = NULL;

  if (pkey == NULL) { e.set(api, Subsystem::SignatureVerifier, RSA_NULL); goto end; }

  ctx = EVP_MD_CTX_new();
  if (ctx == NULL) { e.set(api, Subsystem::SignatureVerifier, CTX_CREATE_FAILED); goto end; }

  r = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey);
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_INIT_FAILED); goto end; }

  r = EVP_DigestVerifyUpdate(ctx, (unsigned char*)message.c_str(), message.size());
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_UPDATE_FAILED); goto end; }

  r = EVP_DigestVerifyFinal(ctx, (unsigned char*)sig.c_str(), sig.size());
  if (r != 1) { e.set(api, Subsystem::SignatureVerifier, DIGEST_VERIFY_FINAL_FAILED); goto end; }

end:
  // Void return type
  EVP_MD_CTX_free(ctx);
}

EVP_PKEY *
create_pkey(basic_Error & e, std::string & modulus, std::string & exponent)
{
  int r;
  BIGNUM * n = NULL;
  BIGNUM * exp = NULL;
  OSSL_PARAM_BLD * param_bld = NULL;
  OSSL_PARAM * params = NULL;
  EVP_PKEY_CTX * ctx = NULL;
  EVP_PKEY * pkey = NULL;

  n = BN_bin2bn((unsigned char*)modulus.c_str(),  modulus.size(), NULL);
  exp = BN_bin2bn((unsigned char*)exponent.c_str(), exponent.size(), NULL);

  if (n == NULL || exp == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  param_bld = OSSL_PARAM_BLD_new();
  if (param_bld == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  r = OSSL_PARAM_BLD_push_BN(param_bld, "n", n);
  if (r != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  r = OSSL_PARAM_BLD_push_BN(param_bld, "e", exp);
  if (r != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  params = OSSL_PARAM_BLD_to_param(param_bld);
  if (params == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  ctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);
  if (ctx == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  r = EVP_PKEY_fromdata_init(ctx);
  if (r != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  r = EVP_PKEY_fromdata(ctx, &pkey, EVP_PKEY_PUBLIC_KEY, params);
  if (r != 1) { e.set(api::main(), errors::Subsystem::SignatureVerifier, __LINE__); goto error; }

  goto end;

error:
  // Void return type
  EVP_PKEY_free(pkey);

end:
  // Void return type
  EVP_PKEY_CTX_free(ctx);

  // Void return type
  OSSL_PARAM_free(params);

  // Void return type
  OSSL_PARAM_BLD_free(param_bld);

  // Void return type
  BN_free(exp);

  // Void return type
  BN_free(n);

  return pkey;
}

} // namespace

SignatureVerifier_OpenSSL3::SignatureVerifier_OpenSSL3(basic_Error & e)
: pkey_(NULL)
{ }

SignatureVerifier_OpenSSL3::~SignatureVerifier_OpenSSL3()
{
  EVP_PKEY_free(this->pkey_);
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
SignatureVerifier_OpenSSL3::set_public_key_base64(basic_Error & e, std::string const& modulus_base64, std::string const& exponent_base64)
{
  if (e) { return; }
  this->set_public_key_base64_(e, modulus_base64, exponent_base64);
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

void
SignatureVerifier_OpenSSL3::set_public_key_base64_(basic_Error & e, std::string const& modulus_base64, std::string const& exponent_base64)
{
  if (e) { return; }

  optional<std::string> modulus  = ::cryptolens_io::v20190401::internal::b64_decode(modulus_base64);
  optional<std::string> exponent = ::cryptolens_io::v20190401::internal::b64_decode(exponent_base64);
  if (!modulus || !exponent) { e.set(api::main(), errors::Subsystem::Base64); return; }

  EVP_PKEY * pkey = create_pkey(e, *modulus, *exponent);
  if (pkey == NULL) { return; }

  pkey_ = pkey;
}

/**
 * This function is used internally by the library and need not be called.
 */
bool
SignatureVerifier_OpenSSL3::verify_message
  ( basic_Error & e
  , std::string const& message
  , std::string const& signature_base64
  )
const
{
  if (e) { return false; }
  if (this->pkey_ == NULL) { e.set(api::main(), errors::Subsystem::SignatureVerifier, RSA_NULL); return false; }

  optional<std::string> sig = ::cryptolens_io::v20190401::internal::b64_decode(signature_base64);
  if (!sig) { e.set(api::main(), errors::Subsystem::Base64); return false; }

  verify(e, this->pkey_, message, *sig);
  if (e) { return false; }

  return true;
}

} // namespace v20190401

} // namespace cryptolens_io
