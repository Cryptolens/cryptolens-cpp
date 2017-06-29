#pragma once

namespace serialkeymanager_com {

class Error {
/*
 * Breaking changes:
 *   - 
 */
private:
  size_t reason_;
public:
  Error(): reason_(NO_ERROR) { }
  Error(Error const& e) = delete;
  Error & operator=(Error const& e) = delete;

  static constexpr size_t NO_ERROR = 0;

  explicit operator bool() const { return reason_ != NO_ERROR; }

  size_t get_reason() const noexcept { return reason_; }

  void reset() { reason_ = NO_ERROR; }

  void set(size_t reason) { reason_ = reason; }

  static size_t constexpr MAKE_REQUEST_CURL_NULL = 1;
  static size_t constexpr MAKE_REQUEST_SETOPT = 2;
  static size_t constexpr MAKE_REQUEST_PERFORM = 3;
  static size_t constexpr VERIFY_RSA_NULL = 4;
  static size_t constexpr VERIFY_RSA_CTX_CREATE = 5;
  static size_t constexpr VERIFY_RSA_PKEY_NEW = 6;
  static size_t constexpr VERIFY_RSA_PKEY_SET = 7;
  static size_t constexpr VERIFY_EVP_INIT = 8;
  static size_t constexpr VERIFY_EVP_UPDATE = 9;
  static size_t constexpr VERIFY_EVP_FINAL = 10;
  static size_t constexpr SET_MODULUS_RSA_NULL = 11;
  static size_t constexpr SET_MODULUS_B64_DECODE_FAILED = 12;
  static size_t constexpr SET_EXPONENT_RSA_NULL = 13;
  static size_t constexpr SET_EXPONENT_B64_DECODE_FAILED = 14;
  static size_t constexpr VERIFY_MESSAGE_RSA_NULL = 15;
  static size_t constexpr VERIFY_MESSAGE_B64_DECODE_FAILED = 16;
  static size_t constexpr BUILD_URL_CURL_NULL = 17;
  static size_t constexpr BUILD_URL_ESCAPE = 18;
  static size_t constexpr RAWLICENSEKEY_MAKE_B64DECODE = 19;
  static size_t constexpr HANDLE_ACTIVATE_JSON_PARSE_FAILED = 20;
  static size_t constexpr HANDLE_ACTIVATE_FAIL_MESSAGE_MISSING = 21;
  static size_t constexpr HANDLE_ACTIVATE_FIXME = 200000;
  static size_t constexpr HANDLE_ACTIVATE_LICENSE_MISSING = 22;
  static size_t constexpr HANDLE_ACTIVATE_SIGNATURE_MISSING = 23;
  static size_t constexpr RAWLICENSEKEY_MAKE_B64_DECODE_FAILED = 24;
//  static size_t constexpr 
};

} // namespace serialkeymanager_com
