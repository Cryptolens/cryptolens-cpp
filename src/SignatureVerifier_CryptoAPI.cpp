#include <memory>
#include <string>

#include "SignatureVerifier_CryptoAPI.hpp"
#include "api.hpp"
#include "base64.hpp"
#include "optional.hpp"

#include <iostream>

namespace cryptolens_io {

namespace v20180502 {

namespace {

constexpr int SIGNATURE_VERIFIER_UNINITIALIZED = 1;
constexpr int CRYPT_ACQUIRE_CONTEXT_FAILED = 2;
constexpr int CRYPT_CREATE_HASH_FAILED = 3;
constexpr int CRYPT_HASH_DATA_FAILED = 4;
constexpr int CRYPT_VERIFY_SIGNATURE_FAILED = 5;
constexpr int CRYPT_IMPORT_KEY_FAILED = 6;
constexpr int DIGEST_VERIFY_FINAL_FAILED = 7;
constexpr int MODULUS_TOO_LARGE = 8;
constexpr int MESSAGE_TOO_LARGE = 9;
constexpr int SIGNATURE_TOO_LARGE = 10;

}

void
verify(basic_Error & e, HCRYPTPROV hProv, HCRYPTKEY hPubKey, std::string const& message, std::string sig)
{
  using namespace errors;
  api::main api;

  constexpr size_t DWORD_MAX = 0xFFFFFFFF;

  if (message.size() > DWORD_MAX) { e.set(api, Subsystem::SignatureVerifier, MESSAGE_TOO_LARGE); return; }
  if (sig.size()     > DWORD_MAX) { e.set(api, Subsystem::SignatureVerifier, SIGNATURE_TOO_LARGE); return; }

  // CryptoAPI assumes things are LSB or whatever, other way around from other people.
  for (size_t i = 0, j = sig.size() - 1; i < j; ++i, --j) { std::swap(sig[i], sig[j]); }

  HCRYPTHASH hHash;
  if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
    DWORD code = GetLastError();
    e.set(api, Subsystem::SignatureVerifier, CRYPT_CREATE_HASH_FAILED, code);
    return;
  }

  if (!CryptHashData(hHash, (const BYTE*)message.c_str(), (DWORD)message.size(), 0)) {
    DWORD code = GetLastError();
    e.set(api, Subsystem::SignatureVerifier, CRYPT_HASH_DATA_FAILED, code);
    return;
  }

  if (!CryptVerifySignature(hHash, (const BYTE*)sig.c_str(), (DWORD)sig.size(), hPubKey, NULL, 0)) {
    DWORD code = GetLastError();
    e.set(api, Subsystem::SignatureVerifier, CRYPT_VERIFY_SIGNATURE_FAILED, code);
    return;
  }
}

SignatureVerifier_CryptoAPI::SignatureVerifier_CryptoAPI() : hProv_{}, hPubKey_{} { }

void SignatureVerifier_CryptoAPI::init(basic_Error & e)
{
  if (!CryptAcquireContext(&this->hProv_, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
    DWORD code = GetLastError();
    e.set(api::main(), 5, CRYPT_ACQUIRE_CONTEXT_FAILED, code);
    return;
  }
}

SignatureVerifier_CryptoAPI::~SignatureVerifier_CryptoAPI()
{
  if (hPubKey_)
  {
    CryptDestroyKey(hPubKey_);
  }

  if (hProv_) {
    CryptReleaseContext(hProv_, 0);
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
SignatureVerifier_CryptoAPI::set_modulus_base64(basic_Error & e, std::string const& modulus_base64)
{
  if (e) { return; }

  this->set_modulus_base64_(e, modulus_base64);
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

void
SignatureVerifier_CryptoAPI::set_modulus_base64_(basic_Error & e, std::string const& modulus_base64)
{
  using namespace errors;
  constexpr size_t DWORD_MAX = 0xFFFFFFFF;

  if (e) { return; }

  if (!hProv_) {
    init(e);
    if (e) { return; }
  }

  optional<std::string> modulus = b64_decode(modulus_base64);
  if (!modulus) { e.set(api::main(), errors::Subsystem::Base64); return; }

  const size_t blobLen = sizeof(BLOBHEADER) + sizeof(RSAPUBKEY) + modulus->size();
  if (blobLen           > DWORD_MAX) { e.set(api::main(), Subsystem::SignatureVerifier, MODULUS_TOO_LARGE); return; }
  if (modulus->size()*8 > DWORD_MAX) { e.set(api::main(), Subsystem::SignatureVerifier, MODULUS_TOO_LARGE); return; }

  // CryptoAPI assumes things are LSB or whatever, other way around from other people.
  for (size_t i = 0, j = modulus->size() - 1; i < j; ++i, --j) { std::swap((*modulus)[i], (*modulus)[j]); }

  std::unique_ptr<BYTE[]> pbKeyBlob(new BYTE[blobLen]);

  BLOBHEADER *blobheader = (BLOBHEADER *)pbKeyBlob.get();
  blobheader->bType = PUBLICKEYBLOB;
  blobheader->bVersion = CUR_BLOB_VERSION;
  blobheader->reserved = 0;
  blobheader->aiKeyAlg = CALG_RSA_KEYX;

  RSAPUBKEY *rsapubkey = (RSAPUBKEY *)(pbKeyBlob.get() + sizeof(BLOBHEADER));
  rsapubkey->magic = 0x31415352;
  rsapubkey->bitlen = (DWORD)(modulus->size() * 8);
  rsapubkey->pubexp = 65537;

  memcpy( pbKeyBlob.get() + sizeof(BLOBHEADER) + sizeof(RSAPUBKEY)
        , (const char *)modulus->c_str()
        , modulus->size()
        );

  if (!CryptImportKey(this->hProv_, pbKeyBlob.get(), (DWORD)blobLen, 0, 0, &this->hPubKey_)) {
    DWORD code = GetLastError();
    e.set(api::main(), Subsystem::SignatureVerifier, CRYPT_IMPORT_KEY_FAILED, code);
    return;
  }
}

/**
 * This function is used internally by the library and need not be called.
 */
bool
SignatureVerifier_CryptoAPI::verify_message
  (basic_Error & e
  , std::string const& message
  , std::string const& signature_base64
  )
  const
{
  if (e) { return false; }
  if (!hProv_ || !hPubKey_) { e.set(api::main(), errors::Subsystem::SignatureVerifier, SIGNATURE_VERIFIER_UNINITIALIZED); return false; }

  optional<std::string> sig = b64_decode(signature_base64);
  if (!sig) { e.set(api::main(), errors::Subsystem::Base64); return false; }

  verify(e, hProv_, hPubKey_, message, *sig);
  if (e) { return false; }

  return true;
}

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
