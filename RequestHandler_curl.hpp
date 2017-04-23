#pragma once

#include <functional>

#include <curl/curl.h>

namespace serialkeymanager_com {

class RequestHandler_curl
{
public:
  RequestHandler_curl();
  ~RequestHandler_curl();

  std::string
  make_request(std::string const& url);

  template<typename Map>
  std::string
  build_url(char const* method, Map const& map)
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

private:
  CURL *curl;
};

} // namespace serialkeymanager_com
