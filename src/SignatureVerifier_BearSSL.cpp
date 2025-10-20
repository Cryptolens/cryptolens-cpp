#include <string>

#include "imports/std/optional"
#include <bearssl_hash.h>

#include "api.hpp"
#include "base64.hpp"
#include "SignatureVerifier_BearSSL.hpp"

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

SignatureVerifier_BearSSL::SignatureVerifier_BearSSL(basic_Error & e)
{
  pk_.n = NULL;
  pk_.nlen = 0;
  pk_.e = NULL;
  pk_.elen = 0;
}

SignatureVerifier_BearSSL::~SignatureVerifier_BearSSL()
{
  delete [] pk_.n;
  delete [] pk_.e;
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
SignatureVerifier_BearSSL::set_modulus_base64(basic_Error & e, std::string const& modulus_base64)
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
SignatureVerifier_BearSSL::set_exponent_base64(basic_Error & e, std::string const& exponent_base64)
{
  if (e) { return; }
  this->set_exponent_base64_(e, exponent_base64);
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_EXPONENT_BASE64); }
}

void
SignatureVerifier_BearSSL::set_modulus_base64_(basic_Error & e, std::string const& modulus_base64)
{
  if (e) { return; }

  optional<std::vector<unsigned char>> modulus = ::cryptolens_io::v20190401::internal::b64_decode(modulus_base64);
  if (!modulus) { e.set(api::main(), errors::Subsystem::Base64); return; }

  if (pk_.n) { delete [] pk_.n; pk_.n = NULL; }

  size_t len = modulus->size();

  try {
    pk_.n = new unsigned char[len];
  } catch (std::bad_alloc const& exception) {
    e.set(api::main(), 3475, 0, 0);
    return;
  }

  memcpy(pk_.n, modulus->data(), len);
  pk_.nlen = len;
}

void
SignatureVerifier_BearSSL::set_exponent_base64_(basic_Error & e, std::string const& exponent_base64)
{
  if (e) { return; }

  optional<std::vector<unsigned char>> exponent = ::cryptolens_io::v20190401::internal::b64_decode(exponent_base64);
  if (!exponent) { e.set(api::main(), errors::Subsystem::Base64); return; }

  if (pk_.e) { delete [] pk_.e; pk_.e = NULL; }

  size_t len = exponent->size();

  try {
    pk_.e = new unsigned char[len];
  } catch (std::bad_alloc const& exception) {
    e.set(api::main(), 3476, 0, 0);
    return;
  }

  memcpy(pk_.e, exponent->data(), len);
  pk_.elen = len;
}

/**
 * This function is used internally by the library and need not be called.
 */
bool
SignatureVerifier_BearSSL::verify_message
  ( basic_Error & e
  , std::vector<unsigned char> const& message
  , std::string const& signature_base64
  )
const
{
  unsigned char hash_out[br_sha256_SIZE];
  br_sha256_context hash_context;

  if (e) { return false; }

  if (pk_.n == NULL || pk_.e == NULL) { e.set(api::main(), 7827, 0, 0); return false; }

  optional<std::vector<unsigned char>> sig = ::cryptolens_io::v20190401::internal::b64_decode(signature_base64);
  if (!sig) { e.set(api::main(), errors::Subsystem::Base64); return false; }

  br_sha256_init(&hash_context);
  br_sha256_update(&hash_context, message.data(), message.size());
  br_sha256_out(&hash_context, hash_out);

  int r = br_rsa_i62_pkcs1_vrfy(sig->data(), sig->size(), BR_HASH_OID_SHA256, br_sha256_SIZE, &pk_, hash_out);
  if (!r) {
    api::main api;
    e.set(api, 1234, 2345);
    return false;
  }

  return true;
}

} // namespace v20190401

} // namespace cryptolens_io
