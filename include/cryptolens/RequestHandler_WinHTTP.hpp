#pragma once

#include <string>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20180502 {

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

} // namespace RequestHandler_WinHTTP

} // namespace errors

/**
 * A request handler that is responsible for making the HTTPS requests
 * to the Serialkeymanager.com Web API. This request handler is build
 * around the Curl library, which is responsible for making the
 * actual HTTPS request.
 *
 * No particular initialization is needed in order to use this
 * RequestHandler.
 */
class RequestHandler_WinHTTP
{
public:
  RequestHandler_WinHTTP();
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  RequestHandler_WinHTTP(RequestHandler_WinHTTP const&) = delete;
  RequestHandler_WinHTTP(RequestHandler_WinHTTP &&) = delete;
  void operator=(RequestHandler_WinHTTP const&) = delete;
  void operator=(RequestHandler_WinHTTP &&) = delete;
#endif
  ~RequestHandler_WinHTTP();

  template<typename Map>
  std::string
  make_request(basic_Error & e, char const* method, Map const& map);

private:
  std::string
  make_request_(basic_Error & e, std::string const& postfields);

  std::string percent_encode_(std::string const& s);

  template<typename Map>
  std::string
  build_url_(basic_Error & e, char const* method, Map const& map);

  template<typename Map>
  std::string
  build_postfields_(basic_Error & e, char const* method, Map const& map);
};

/**
 * This method is used internally in the library and need not be called by the user.
 */
template<typename Map>
std::string
RequestHandler_WinHTTP::make_request(basic_Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }

  std::string postfields = build_postfields_(e, method, map);
  return make_request_(e, postfields);
}

template<typename Map>
std::string
RequestHandler_WinHTTP::build_postfields_(basic_Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }

  using namespace errors::RequestHandler_WinHTTP;

  std::string s{""};

  bool first = true;
  char separator = '&';
  for (auto& x : map) {
    if (first) { first = false; }
    else       { s += separator; }

    std::string res;
    res = percent_encode_(x.first);
    s += res;

    s += '=';

    res = percent_encode_(x.second);
    s += res;
  }

  return s;
}

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
