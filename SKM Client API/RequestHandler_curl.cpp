#include <curl/curl.h>

#include "RequestHandler_curl.hpp"

namespace serialkeymanager_com {

size_t
handle_response(char * ptr, size_t size, size_t nmemb, void *userdata)
{
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
RequestHandler_curl::make_request_(Error & e, std::string const& url)
{
  if (e) { return ""; }
  if (!this->curl) { e.set(Subsystem::RequestHandler); return ""; }

  std::string response;
  CURLcode cc;

  cc = curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, handle_response);
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&response);
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }

  // FIXME: Temporary addition since we are doing cryptographic check
  //        in the library aswell.
  cc = curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0);
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0);
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }

  cc = curl_easy_perform(this->curl);
  if (cc != CURLE_OK) { e.set(Subsystem::RequestHandler); return ""; }

  return response;
}

RequestHandler_curl::~RequestHandler_curl()
{
  if (this->curl) {
    curl_easy_cleanup(this->curl);
  }
}

} // namespace serialkeymanager_com
