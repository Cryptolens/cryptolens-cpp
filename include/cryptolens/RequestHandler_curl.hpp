#pragma once

#include <string>

#include "imports/curl/curl.h"

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace RequestHandler_curl {

int constexpr CURL_NULL = 1;
int constexpr ESCAPE = 2;
int constexpr SETOPT_URL = 3;
int constexpr SETOPT_WRITEFUNCTION = 4;
int constexpr SETOPT_WRITEDATA = 5;
int constexpr SETOPT_VERIFYPEER = 6;
int constexpr SETOPT_VERIFYHOST = 7;
int constexpr PERFORM = 8;
int constexpr SETOPT_POSTFIELDS = 9;

} // namespace RequestHandler_curl

} // namespace errors

/**
 * A request handler that is responsible for making the HTTPS requests
 * to the Cryptolens Web API. This request handler is build
 * around the Curl library, which is responsible for making the
 * actual HTTPS request.
 *
 * No particular initialization is needed in order to use this
 * RequestHandler.
 */
class RequestHandler_curl
{
public:
  RequestHandler_curl();
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  RequestHandler_curl(RequestHandler_curl const&) = delete;
  RequestHandler_curl(RequestHandler_curl &&) = delete;
  void operator=(RequestHandler_curl const&) = delete;
  void operator=(RequestHandler_curl &&) = delete;
#endif
  ~RequestHandler_curl();

  template<typename Map>
  std::string
  make_request(basic_Error & e, char const* method, Map const& map);

private:
  CURL *curl;

  std::string
  make_request_(basic_Error & e, std::string const& url, std::string const& postfields);

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
RequestHandler_curl::make_request(basic_Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }

  std::string url = build_url_(e, method, map);
  std::string postfields = build_postfields_(e, method, map);
  return make_request_(e, url, postfields);
}

template<typename Map>
std::string
RequestHandler_curl::build_url_(basic_Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }

  using namespace errors::RequestHandler_curl;
  api::main api;

  if (!this->curl) { e.set(api, errors::Subsystem::RequestHandler, CURL_NULL); return ""; }

  char* res;
  std::string s{"https://app.cryptolens.io/api/key/"};

  res = curl_easy_escape(curl, method, 0);
  if (!res) { e.set(api, errors::Subsystem::RequestHandler, ESCAPE); return ""; }
  s += res;
  curl_free(res);

  return s;
}

template<typename Map>
std::string
RequestHandler_curl::build_postfields_(basic_Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }

  using namespace errors::RequestHandler_curl;
  api::main api;

  if (!this->curl) { e.set(api, errors::Subsystem::RequestHandler, CURL_NULL); return ""; }

  char* res;
  std::string s{""};

  bool first = true;
  char separator = '&';
  for (auto& x : map) {
    if (first) { first = false; }
    else       { s += separator; }

    res = curl_easy_escape(curl, x.first.c_str(), 0);
    if (!res) { e.set(api, errors::Subsystem::RequestHandler, ESCAPE); return ""; }
    s += res;
    curl_free(res);

    s += '=';

    res = curl_easy_escape(curl, x.second.c_str(), 0);
    if (!res) { e.set(api, errors::Subsystem::RequestHandler, ESCAPE); return ""; }
    s += res;
    curl_free(res);
  }

  return s;
}

} // namespace v20190401

namespace latest {

namespace errors {

namespace RequestHandler_curl = ::cryptolens_io::v20190401::errors::RequestHandler_curl;

} // namespace errors

using RequestHandler_curl = ::cryptolens_io::v20190401::RequestHandler_curl;

} // namespace latest

} // namespace cryptolens_io
