#pragma once

#include <functional>

#include <curl/curl.h>

namespace serialkeymanager_com {

// A request handler for making requests to the SKM Web API built
// around the curl library.
class RequestHandler_curl
{
public:
  RequestHandler_curl();
  ~RequestHandler_curl();

  template<typename Map>
  std::string
  make_request(char const* method, Map const& map)
  {
    std::string url = build_url_(method, map);
    return make_request_(url);
  }

private:
  CURL *curl;

  std::string
  make_request_(std::string const& url);

  template<typename Map>
  std::string
  build_url_(char const* method, Map const& map)
  {
    char* res;
    std::string s{"https://serialkeymanager.com/api/key/"};

    res = curl_easy_escape(curl, method, 0);
    s += res;
    curl_free(res);

    bool first = true;
    char separator = '?';
    for (auto& e : map) {
      s += separator;
      if (first) {
        first = false;
        separator = '&';
      }

      res = curl_easy_escape(curl, e.first.c_str(), 0);
      s += res;
      curl_free(res);

      s += '=';

      res = curl_easy_escape(curl, e.second.c_str(), 0);
      s += res;
      curl_free(res);
    }

    return s;
  }
};

} // namespace serialkeymanager_com
