#pragma once

#include <string>

#include <curl/curl.h>

#include "Error.hpp"

namespace serialkeymanager_com {

namespace rhcerr {

int constexpr CURL_NULL = 1;
int constexpr ESCAPE = 2;
int constexpr SETOPT_URL = 3;
int constexpr SETOPT_WRITEFUNCTION = 4;
int constexpr SETOPT_WRITEDATA = 5;
int constexpr SETOPT_VERIFYPEER = 6;
int constexpr SETOPT_VERIFYHOST = 7;
int constexpr PERFORM = 8;

}

/**
 * A request handler that is responsible for making the HTTPS requests
 * to the Serialkeymanager.com Web API. This request handler is build
 * around the Curl library, which is responsible for making the
 * actual HTTPS request.
 */
class RequestHandler_curl
{
public:
  RequestHandler_curl();
  ~RequestHandler_curl();

  template<typename Map>
  std::string
  make_request(Error & e, char const* method, Map const& map);

private:
  CURL *curl;

  std::string
  make_request_(Error & e, std::string const& url);

  template<typename Map>
  std::string
  build_url_(Error & e, char const* method, Map const& map);
};

/**
 * This method is used internally in the library and need not be called by the user.
 */
template<typename Map>
std::string
RequestHandler_curl::make_request(Error & e, char const* method, Map const& map)
{
  std::string url = build_url_(e, method, map);
  return make_request_(e, url);
}

template<typename Map>
std::string
RequestHandler_curl::build_url_(Error & e, char const* method, Map const& map)
{
  if (e) { return ""; }
  if (!this->curl) { e.set(Subsystem::RequestHandler, rhcerr::CURL_NULL); return ""; }

  char* res;
  std::string s{"https://serialkeymanager.com/api/key/"};

  res = curl_easy_escape(curl, method, 0);
  if (!res) { e.set(Subsystem::RequestHandler, rhcerr::ESCAPE); return ""; }
  s += res;
  curl_free(res);

  bool first = true;
  char separator = '?';
  for (auto& x : map) {
    s += separator;
    if (first) {
      first = false;
      separator = '&';
    }

    res = curl_easy_escape(curl, x.first.c_str(), 0);
    if (!res) { e.set(Subsystem::RequestHandler, rhcerr::ESCAPE); return ""; }
    s += res;
    curl_free(res);

    s += '=';

    res = curl_easy_escape(curl, x.second.c_str(), 0);
    if (!res) { e.set(Subsystem::RequestHandler, rhcerr::ESCAPE); return ""; }
    s += res;
    curl_free(res);
  }

  return s;
}

} // namespace serialkeymanager_com
