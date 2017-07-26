#pragma once

namespace serialkeymanager_com {

namespace Subsystem {

/*
 * Ok means no error has occured
 * All other values indicate error
 */
int constexpr Ok = 0;
int constexpr Main = 1;
int constexpr Json = 2;
int constexpr Base64 = 3;
int constexpr RequestHandler = 4;
int constexpr SignatureVerifier = 5;

} // namespace Subsystem

namespace Call {

/*
 * Which method caused the error
 */

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

/**
 * This error is used by all methods in this library that can fail.
 *
 * Several things are provided:
 *   - Chaining multiple calls without having to check for error
 *     after every call.
 *   - Still keep track of which call caused the error, if neccesary.
 *     Maybe add a user version of this tracking too.
 *   - Summary information about the error
 *   - Detailed information for debugging
 */
class Error {
private:
  int    call_;
  int    subsystem_;
  int    reason_;
  size_t extra_;
public:
  Error(): call_(0), subsystem_(Subsystem::Ok), reason_(0), extra_(0) { }
  Error(Error const& e) = delete;
  Error & operator=(Error const& e) = delete;

  /**
   * Explicit conversion to bool allowing the object to be used in e.g.
   * control flow statements.
   *
   * Returns true if an error has occured, false otherwise.
   */
  explicit operator bool() const { return subsystem_ != Subsystem::Ok; }

  int get_subsystem() const noexcept { return subsystem_; }
  int get_reason() const noexcept { return reason_; }
  size_t get_extra() const noexcept { return extra_; }

  /**
   * Can be used to reset the Error object to the initial state.
   *
   * Can be used for reusing the same Error object after an error
   * has occured.
   */
  void reset() { subsystem_ = Subsystem::Ok; reason_ = 0; extra_ = 0; }

  void set(int subsystem) { subsystem_ = subsystem; }
  void set(int subsystem, int reason) { subsystem_ = subsystem; reason_ = reason; }
  void set(int subsystem, int reason, size_t extra) { subsystem_ = subsystem; reason_ = reason; extra_ = extra; }
  void set_call(int call) { call_ = call; }
};

} // namespace serialkeymanager_com
