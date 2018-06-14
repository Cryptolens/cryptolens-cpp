#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include <Windows.h>
#include <winhttp.h>

#include "RequestHandler_WinHTTP.hpp"

namespace cryptolens_io {

namespace v20180502 {

RequestHandler_WinHTTP::RequestHandler_WinHTTP() { }

RequestHandler_WinHTTP::~RequestHandler_WinHTTP() { }

std::string
RequestHandler_WinHTTP::make_request_(basic_Error & e, std::string const& postfields)
{
  if (e) { return ""; }

  using namespace errors;
  namespace err = errors::RequestHandler_WinHTTP;
  api::main api;

  HINTERNET hSession = NULL,
            hConnect = NULL,
            hRequest = NULL;
  BOOL result = FALSE;
  DWORD bytes;
  std::string response;
  std::vector<char> resp_temp;

  if (postfields.size() > 0xFFFFFFFF) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_POSTFIELDS_TOO_LARGE, GetLastError()); goto cleanup; }

  hSession = WinHttpOpen(L"Cryptolens SKM Client WinHTTP/CryptoAPI"
                                  , WINHTTP_ACCESS_TYPE_DEFAULT_PROXY
                                  , WINHTTP_NO_PROXY_NAME
                                  , WINHTTP_NO_PROXY_BYPASS
                                  , 0);
  if (!hSession) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_OPEN_FAILED); goto cleanup; }

  hConnect = WinHttpConnect(hSession, L"app.cryptolens.io", INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_CONNECT_FAILED, GetLastError()); goto cleanup; }

  hRequest = WinHttpOpenRequest( hConnect
                               , L"POST"
                               , L"/api/key/Activate"
                               , NULL
                               , WINHTTP_NO_REFERER
                               , WINHTTP_DEFAULT_ACCEPT_TYPES
                               , WINHTTP_FLAG_SECURE);
  if (!hRequest) { e.set(api, Subsystem::RequestHandler, err::WINHTTP_OPEN_REQUEST_FAILED, GetLastError()); goto cleanup; }

  result =  WinHttpSendRequest( hRequest
                               , L"Content-Type: application/x-www-form-urlencoded"
                               , -1
                               , (LPVOID *)postfields.c_str()
                               , (DWORD)postfields.size()
                               , (DWORD)postfields.size()
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

  if (e) { return ""; }
  return response;
}


/* FROM CURL */
static bool Curl_isunreserved(unsigned char in)
{
  switch (in) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case '-': case '.': case '_': case '~':
      return 1;
    default:
      break;
  }
  return 0;
}

std::string
RequestHandler_WinHTTP::percent_encode_(std::string const& s)
{
  std::string r;

  char translate[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (auto& x : s) {
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

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
