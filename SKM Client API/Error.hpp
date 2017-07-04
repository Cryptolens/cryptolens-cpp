#pragma once

namespace serialkeymanager_com {

namespace Subsystem {

int constexpr Ok = 0;
int constexpr Main = 1;
int constexpr Json = 2;
int constexpr Base64 = 3;
int constexpr RequestHandler = 4;
int constexpr SignatureVerifier = 5;

} // namespace Subsystem

namespace Call {

int constexpr BASIC_SKM_ACTIVATE = 1;
int constexpr BASIC_SKM_HANDLE_ACTIVATE = 2;
int constexpr SIGNATURE_VERIFIER_SET_EXPONENT_BASE64 = 3;
int constexpr SIGNATURE_VERIFIER_SET_MODULUS_BASE64 = 4;

} // namespace Call

// Errors for the Main subsystem
namespace Main {

constexpr int UNKNOWN_SERVER_REPLY      = 1;
constexpr int INVALID_ACCESS_TOKEN      = 2;
constexpr int ACCESS_DENIED             = 3;
constexpr int INCORRECT_INPUT_PARAMETER = 4;
constexpr int PRODUCT_NOT_FOUND         = 5;
constexpr int KEY_NOT_FOUND             = 6;
constexpr int KEY_BLOCKED               = 7;
constexpr int DEVICE_LIMIT_REACHED      = 8;

} // namespace Main

class Error {
/*
 * Breaking changes:
 *   - 
 */
private:
  int    call_;
  int    subsystem_;
  int    reason_;
  size_t extra_;
public:
  Error(): call_(0), subsystem_(Subsystem::Ok), reason_(0), extra_(0) { }
  Error(Error const& e) = delete;
  Error & operator=(Error const& e) = delete;

  explicit operator bool() const { return subsystem_ != Subsystem::Ok; }

  int get_subsystem() const noexcept { return subsystem_; }
  int get_reason() const noexcept { return reason_; }
  size_t get_extra() const noexcept { return extra_; }

  void reset() { subsystem_ = Subsystem::Ok; reason_ = 0; extra_ = 0; }

  void set(int subsystem) { subsystem_ = subsystem; }
  void set(int subsystem, int reason) { subsystem_ = subsystem; reason_ = reason; }
  void set(int subsystem, int reason, size_t extra) { subsystem_ = subsystem; reason_ = reason; extra_ = extra; }
  void set_call(int call) { call_ = call; }

#if 0
  static int constexpr MAKE_REQUEST_CURL_NULL = 1;
  static int constexpr MAKE_REQUEST_SETOPT = 2;
  static int constexpr MAKE_REQUEST_PERFORM = 3;
  static int constexpr VERIFY_RSA_NULL = 4;
  static int constexpr VERIFY_RSA_CTX_CREATE = 5;
  static int constexpr VERIFY_RSA_PKEY_NEW = 6;
  static int constexpr VERIFY_RSA_PKEY_SET = 7;
  static int constexpr VERIFY_EVP_INIT = 8;
  static int constexpr VERIFY_EVP_UPDATE = 9;
  static int constexpr VERIFY_EVP_FINAL = 10;
  static int constexpr SET_MODULUS_RSA_NULL = 11;
  static int constexpr SET_MODULUS_B64_DECODE_FAILED = 12;
  static int constexpr SET_EXPONENT_RSA_NULL = 13;
  static int constexpr SET_EXPONENT_B64_DECODE_FAILED = 14;
  static int constexpr VERIFY_MESSAGE_RSA_NULL = 15;
  static int constexpr VERIFY_MESSAGE_B64_DECODE_FAILED = 16;
  static int constexpr BUILD_URL_CURL_NULL = 17;
  static int constexpr BUILD_URL_ESCAPE = 18;
  static int constexpr RAWLICENSEKEY_MAKE_B64DECODE = 19;
  static int constexpr HANDLE_ACTIVATE_JSON_PARSE_FAILED = 20;
  static int constexpr HANDLE_ACTIVATE_FAIL_MESSAGE_MISSING = 21;
  static int constexpr HANDLE_ACTIVATE_FIXME = 200000;
  static int constexpr HANDLE_ACTIVATE_LICENSE_MISSING = 22;
  static int constexpr HANDLE_ACTIVATE_SIGNATURE_MISSING = 23;
  static int constexpr RAWLICENSEKEY_MAKE_B64_DECODE_FAILED = 24;
  static int constexpr LICENSEKEY_MAKE_JSON_PARSE_FAILED = 25;
  static int constexpr LICENSEKEY_MAKE_MANDATORY_MISSING = 26;
//  static size_t constexpr 
#endif
};

} // namespace serialkeymanager_com
