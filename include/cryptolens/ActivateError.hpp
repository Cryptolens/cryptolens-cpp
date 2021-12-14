#pragma once

#include <exception>

namespace cryptolens_io {

namespace v20190401 {

class ActivateError : public std::exception {
private:
  ActivateError(int reason);

  int reason_;
public:
  static const int UNKNOWN_SERVER_REPLY      = 1;
  static const int INVALID_ACCESS_TOKEN      = 2;
  static const int ACCESS_DENIED             = 3;
  static const int INCORRECT_INPUT_PARAMETER = 4;
  static const int PRODUCT_NOT_FOUND         = 5;
  static const int KEY_NOT_FOUND             = 6;
  static const int KEY_BLOCKED               = 7;
  static const int DEVICE_LIMIT_REACHED      = 8;
  static const int KEY_EXPIRED               = 9;
  static const int MACHINE_CODE_NOT_ACTIVATED_OR_NO_KEY_ACTIVATION = 10;

  static ActivateError from_reason(int reason);

  static ActivateError from_server_response(char const* server_response);

  int get_reason() const noexcept;

  virtual const char * what() const noexcept;
};

} // namespace v20190401

namespace v20180502 {

using ::cryptolens_io::v20190401::ActivateError;

} // namespace v20180502

namespace latest {

using ::cryptolens_io::v20190401::ActivateError;

} // namespace latest

} // namespace cryptolens_io
