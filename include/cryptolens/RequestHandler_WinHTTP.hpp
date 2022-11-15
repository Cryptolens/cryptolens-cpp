#pragma once

#include <string>

#include "imports/windows/Windows.h"
#include "imports/windows/winhttp.h"

#include "api.hpp"
#include "basic_Error.hpp"
#include "RequestHandler_v20190401_to_v20180502.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace RequestHandler_WinHTTP {

int constexpr WINHTTP_OPEN_FAILED = 1;
int constexpr WINHTTP_CONNECT_FAILED = 2;
int constexpr WINHTTP_OPEN_REQUEST_FAILED = 3;
int constexpr WINHTTP_SEND_REQUEST_FAILED = 4;
int constexpr WINHTTP_RECIEVE_RESPONSE_FAILED = 5;
int constexpr WINHTTP_QUERY_DATA_AVAILABLE_FAILED = 6;
int constexpr WINHTTP_READ_DATA_FAILED = 7;
int constexpr WINHTTP_POSTFIELDS_TOO_LARGE = 8;
int constexpr MULTIBYTETOWIDE_ENDPOINT = 9;
int constexpr MULTIBYTETOWIDE_HOST = 10;
int constexpr WINHTTP_SET_TIMEOUTS = 11;

} // namespace RequestHandler_WinHTTP

} // namespace errors

class RequestHandler_WinHTTP_PostBuilder {
public:
  RequestHandler_WinHTTP_PostBuilder(char const* host, char const* endpoint, int resolve_timeout_ms, int connect_timeout_ms, int send_timeout_ms, int receive_timeout_ms);

  RequestHandler_WinHTTP_PostBuilder &
  add_argument(basic_Error & e, char const* key, char const* value);

  std::string
  make(basic_Error & e);

private:
  char separator_;
  std::string postfields_;
  char const* host_;
  char const* endpoint_;

  int resolve_timeout_ms_;
  int connect_timeout_ms_;
  int send_timeout_ms_;
  int receive_timeout_ms_;
};

/**
 * A request handler that is responsible for making the HTTPS requests
 * to the Cryptolens Web API. This request handler is build
 * around the Curl library, which is responsible for making the
 * actual HTTPS request.
 *
 * No particular initialization is needed in order to use this
 * RequestHandler.
 */
class RequestHandler_WinHTTP
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  RequestHandler_WinHTTP(basic_Error & e);
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  RequestHandler_WinHTTP(RequestHandler_WinHTTP const&) = delete;
  RequestHandler_WinHTTP(RequestHandler_WinHTTP &&) = delete;
  void operator=(RequestHandler_WinHTTP const&) = delete;
  void operator=(RequestHandler_WinHTTP &&) = delete;
#endif
  ~RequestHandler_WinHTTP();

  using PostBuilder = RequestHandler_WinHTTP_PostBuilder;

  PostBuilder
  post_request(basic_Error& e, char const* host, char const* endpoint);

  void set_timeout(basic_Error & e, long timeout);
  void set_timeout(basic_Error& e, int resolve_timeout_ms, int connect_timeout_ms, int send_timeout_ms, int receive_timeout_ms);

private:
  int resolve_timeout_ms_;
  int connect_timeout_ms_;
  int send_timeout_ms_;
  int receive_timeout_ms_;
};

} // namespace v20190401

namespace latest {

namespace errors {

namespace RequestHandler_WinHTTP = ::cryptolens_io::v20190401::errors::RequestHandler_WinHTTP;

} // namespace errors

using RequestHandler_WinHTTP = ::cryptolens_io::v20190401::RequestHandler_WinHTTP;

} // namespace latest

namespace v20180502 {

namespace errors {

namespace RequestHandler_WinHTTP = ::cryptolens_io::v20190401::errors::RequestHandler_WinHTTP;

} // namespace errors

using RequestHandler_WinHTTP = ::cryptolens_io::internal::RequestHandler_v20190401_to_v20180502<::cryptolens_io::v20190401::RequestHandler_WinHTTP>;

} // namespace v20180502

} // namespace cryptolens_io
