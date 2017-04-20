#include <functional>

#include <curl/curl.h>

#include "RequestHandler_curl.hpp"

namespace serialkeymanager_com {

size_t
handle_response(char * ptr, size_t size, size_t nmemb, void *userdata)
{
#if 0
  std::cout << "size: " << size << std::endl;
  std::cout << "nmemb: " << nmemb << std::endl;
  std::cout << std::endl;
  // TODO: I guess size*nmemb may overflow? Or why this way of reporting?
  //       Idk, we need to return the number of actually taken care of,
  //       according to https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
  //       so overflow doesn't make any sense?
#endif
  std::string current{ptr, size*nmemb};

  std::string *response = (std::string *)userdata;
  *response += current;

  return size*nmemb;
}

RequestHandler_curl::RequestHandler_curl()
{
  this->curl = curl_easy_init();
}

std::string
RequestHandler_curl::make_request( std::string const& url)
{
  std::string response;

  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, handle_response);
  curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&response);
  curl_easy_perform(this->curl);

  return response;
}

RequestHandler_curl::~RequestHandler_curl()
{
  curl_easy_cleanup(this->curl);
}

} // namespace serialkeymanager_com
