#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include <Windows.h>
#include <winhttp.h>

#include "RequestHandler_WinHTTP.hpp"
#include "../third_party/curl/isunreserved.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

std::string
percent_encode(std::string const& s);

} // namespace internal

/*
 * RequestHandler_WinHTTP
 */

RequestHandler_WinHTTP::RequestHandler_WinHTTP(basic_Error & e)
: resolve_timeout_ms_(0), connect_timeout_ms_(60000),
  send_timeout_ms_(30000), receive_timeout_ms_(30000)
{}

RequestHandler_WinHTTP::~RequestHandler_WinHTTP() {}

RequestHandler_WinHTTP::PostBuilder
RequestHandler_WinHTTP::post_request(basic_Error& e, char const* host, char const* endpoint)
{
  return RequestHandler_WinHTTP_PostBuilder(host, endpoint, resolve_timeout_ms_, connect_timeout_ms_, send_timeout_ms_, receive_timeout_ms_);
}

void
RequestHandler_WinHTTP::set_timeout(basic_Error & e, long timeout)
{
  set_timeout(e, timeout, timeout, timeout, timeout);
}

void
RequestHandler_WinHTTP::set_timeout(basic_Error & e, int resolve_timeout_ms, int connect_timeout_ms, int send_timeout_ms, int receive_timeout_ms)
{
  if (e) { return; }

  resolve_timeout_ms_ = resolve_timeout_ms;
  connect_timeout_ms_ = connect_timeout_ms;
  send_timeout_ms_    = send_timeout_ms;
  receive_timeout_ms_ = receive_timeout_ms;
}

/*
 * RequestHandler_WinHTTP_PostBuilder
 */

RequestHandler_WinHTTP_PostBuilder::RequestHandler_WinHTTP_PostBuilder(char const* host, char const* endpoint, int resolve_timeout_ms, int connect_timeout_ms, int send_timeout_ms, int receive_timeout_ms)
: separator_(' '), postfields_(), host_(host), endpoint_(endpoint),
  resolve_timeout_ms_(resolve_timeout_ms), connect_timeout_ms_(connect_timeout_ms),
  send_timeout_ms_(send_timeout_ms), receive_timeout_ms_(receive_timeout_ms)
{}

RequestHandler_WinHTTP_PostBuilder &
RequestHandler_WinHTTP_PostBuilder::add_argument(basic_Error & e, char const* key, char const* value)
{
  if (e) { return *this; }

  using namespace errors::RequestHandler_WinHTTP;

  // TODO: separator_ is initialized to ' ', move this to a constant?
  if (separator_ == ' ') { separator_ = '&'; }
  else                   { postfields_ += separator_; }

  std::string res;
  res = internal::percent_encode(key);
  postfields_ += res;

  postfields_ += '=';

  res = internal::percent_encode(value);
  postfields_ += res;

  return *this;
}


void cryptolens_secure_failure_callback(
  HINTERNET hInternet,
  DWORD_PTR dwContext,
  DWORD dwInternetStatus,
  LPVOID lpvStatusInformation,
  DWORD dwStatusInformationLength
)
{
  DWORD* ssl_error = (DWORD*)dwContext;
  *ssl_error = *((DWORD*)lpvStatusInformation);
}


std::string
RequestHandler_WinHTTP_PostBuilder::make(basic_Error & e)
{
  if (e) { return ""; }

  char const* host = host_;

  using namespace errors;
  namespace err = errors::RequestHandler_WinHTTP;
  api::main api;

  LPWSTR endpoint_w = nullptr;
  LPWSTR host_w = nullptr;

  HINTERNET hSession = NULL,
            hConnect = NULL,
            hRequest = NULL;
  BOOL result = FALSE;
  DWORD bytes;
  DWORD ssl_error = 123;
  DWORD_PTR loc_ssl_error = (DWORD_PTR)&ssl_error;
  std::string response;
  std::vector<char> resp_temp;

  if (postfields_.size() > 0xFFFFFFFF) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_POSTFIELDS_TOO_LARGE, GetLastError()); goto cleanup; }

  int size, res;
  size = MultiByteToWideChar(CP_UTF8, 0, endpoint_, -1, NULL, 0);
  if (size == 0) { e.set(api, Subsystem::RequestHandler, err::MULTIBYTETOWIDE_ENDPOINT, GetLastError()); goto cleanup; }
  endpoint_w = new wchar_t[size];
  res = MultiByteToWideChar(CP_UTF8, 0, endpoint_, -1, endpoint_w, size);
  if (res == 0) { e.set(api, Subsystem::RequestHandler, err::MULTIBYTETOWIDE_ENDPOINT, GetLastError()); goto cleanup; }

  size = MultiByteToWideChar(CP_UTF8, 0, host, -1, NULL, 0);
  if (size == 0) { e.set(api, Subsystem::RequestHandler, err::MULTIBYTETOWIDE_HOST, GetLastError()); goto cleanup; }
  host_w = new wchar_t[size];
  res = MultiByteToWideChar(CP_UTF8, 0, host, -1, host_w, size);
  if (res == 0) { e.set(api, Subsystem::RequestHandler, err::MULTIBYTETOWIDE_HOST, GetLastError()); goto cleanup; }


  hSession = WinHttpOpen( L"Cryptolens WinHTTP"
                        , WINHTTP_ACCESS_TYPE_DEFAULT_PROXY
                        , WINHTTP_NO_PROXY_NAME
                        , WINHTTP_NO_PROXY_BYPASS
                        , 0);
  if (!hSession) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_OPEN_FAILED, GetLastError()); goto cleanup; }

  if (!WinHttpSetOption(hSession, WINHTTP_OPTION_CONTEXT_VALUE, &loc_ssl_error, sizeof(&loc_ssl_error))) {
    e.set(api, Subsystem::RequestHandler, 42420, GetLastError());
    goto cleanup;
  }

  if (WinHttpSetStatusCallback(hSession, &cryptolens_secure_failure_callback, WINHTTP_CALLBACK_FLAG_SECURE_FAILURE, NULL) == WINHTTP_INVALID_STATUS_CALLBACK) {
    e.set(api, Subsystem::RequestHandler, 42421, GetLastError());
    goto cleanup;
  }

  result = WinHttpSetTimeouts(hSession, resolve_timeout_ms_, connect_timeout_ms_, send_timeout_ms_, receive_timeout_ms_);
  if (!result) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_SET_TIMEOUTS, GetLastError()); goto cleanup; }

  hConnect = WinHttpConnect(hSession, host_w, INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_CONNECT_FAILED, GetLastError()); goto cleanup; }

  hRequest = WinHttpOpenRequest( hConnect
                               , L"POST"
                               , endpoint_w
                               , NULL
                               , WINHTTP_NO_REFERER
                               , WINHTTP_DEFAULT_ACCEPT_TYPES
                               , WINHTTP_FLAG_SECURE);
  if (!hRequest) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_OPEN_REQUEST_FAILED, GetLastError()); goto cleanup; }

  result =  WinHttpSendRequest( hRequest
                               , L"Content-Type: application/x-www-form-urlencoded"
                               , -1L
                               , (LPVOID *)postfields_.c_str()
                               , (DWORD)postfields_.size()
                               , (DWORD)postfields_.size()
                               , NULL);
  if (!result) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_SEND_REQUEST_FAILED, GetLastError()); goto cleanup; }

  result = WinHttpReceiveResponse(hRequest, NULL);
  if (!result) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_RECIEVE_RESPONSE_FAILED, GetLastError()); goto cleanup; }

  do {
    bytes = 0;
    result = WinHttpQueryDataAvailable(hRequest, &bytes);
    if (!result) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_QUERY_DATA_AVAILABLE_FAILED, GetLastError()); goto cleanup; }

    resp_temp.resize((size_t)bytes+1, ' '); // Add one so length isn't zero which formally would give UB for operator[] below
    result = WinHttpReadData(hRequest, &resp_temp[0], bytes, NULL);
    if (!result) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_READ_DATA_FAILED, GetLastError()); goto cleanup; }

    std::copy(resp_temp.begin(), resp_temp.begin() + bytes, std::back_inserter(response));
  } while (bytes > 0);

cleanup:
  if(hRequest) { WinHttpCloseHandle(hRequest); }
  if(hConnect) { WinHttpCloseHandle(hConnect); }
  if(hSession) { WinHttpCloseHandle(hSession); }

  if (ssl_error != 0) {
    e.set(api, Subsystem::RequestHandler, 42422, ssl_error);
  }

  delete [] endpoint_w;
  delete [] host_w;

  if (e) { return ""; }
  return response;
}


namespace internal {

std::string
percent_encode(std::string const& s)
{
  std::string r;

  char translate[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (char const& x : s) {
    if (Curl_isunreserved(x)) {
      r += x;
    }
    else {
      r += '%';
      r += translate[(x >> 4) & 0xF];
      r += translate[x & 0xF];
    }
  }

  return r;
}

} // namespace internal

} // namespace v20190401

} // namespace cryptolens_io
