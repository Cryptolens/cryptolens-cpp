#pragma once

#include <exception>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

class ActivateError : public std::exception {
private:
  ActivateError(int reason);

  int reason_;
public:
  static const int UNKNOWN_SERVER_REPLY      = ::cryptolens_io::v20190401::errors::Main::UNKNOWN_SERVER_REPLY;
  static const int INVALID_ACCESS_TOKEN      = ::cryptolens_io::v20190401::errors::Main::INVALID_ACCESS_TOKEN;
  static const int ACCESS_DENIED             = ::cryptolens_io::v20190401::errors::Main::ACCESS_DENIED;
  static const int INCORRECT_INPUT_PARAMETER = ::cryptolens_io::v20190401::errors::Main::INCORRECT_INPUT_PARAMETER;
  static const int PRODUCT_NOT_FOUND         = ::cryptolens_io::v20190401::errors::Main::PRODUCT_NOT_FOUND;
  static const int KEY_NOT_FOUND             = ::cryptolens_io::v20190401::errors::Main::KEY_NOT_FOUND;
  static const int KEY_BLOCKED               = ::cryptolens_io::v20190401::errors::Main::KEY_BLOCKED;
  static const int DEVICE_LIMIT_REACHED      = ::cryptolens_io::v20190401::errors::Main::DEVICE_LIMIT_REACHED;
  static const int KEY_EXPIRED               = ::cryptolens_io::v20190401::errors::Main::KEY_EXPIRED;
  static const int MACHINE_CODE_NOT_ACTIVATED_OR_NO_KEY_ACTIVATION = ::cryptolens_io::v20190401::errors::Main::MACHINE_CODE_NOT_ACTIVATED_OR_NO_KEY_ACTIVATION;

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
