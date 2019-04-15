#include <curl/curl.h>

#ifdef CRYPTOLENS_CURL_EMBED_CACERTS
#include <openssl/ssl.h>
#include <vector>
#endif /* CRYPTOLENS_CURL_EMBED_CACERTS */

#include "RequestHandler_curl.hpp"

namespace cryptolens_io {

namespace v20190401 {

size_t
handle_response(char * ptr, size_t size, size_t nmemb, void *userdata)
{
  std::string current{ptr, size*nmemb};

  std::string *response = (std::string *)userdata;
  *response += current;

  return size*nmemb;
}

#ifdef CRYPTOLENS_CURL_EMBED_CACERTS

namespace cacerts {

extern
std::vector<std::string> pems;

} // namespace cacerts

static
CURLcode
sslctx_function_setup_cacerts(CURL *curl, void *sslctx, void *parm)
{
  /* This function is originally based on the provided code in the
   * man page for CURLOPT_SSL_CTX_FUNCTION available in the file
   * docs/libcurl/opts/CURLOPT_SSL_CTX_FUNCTION.3 as part of the
   * archive for curl 7.55.0. This file is distributed under the
   * with the following terms:
   *
   **************************************************************************
   *                                  _   _ ____  _
   *  Project                     ___| | | |  _ \| |
   *                             / __| | | | |_) | |
   *                            | (__| |_| |  _ <| |___
   *                             \___|\___/|_| \_\_____|
   *
   * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
   *
   * This software is licensed as described in the file COPYING, which
   * you should have received as part of this distribution. The terms
   * are also available at https://curl.haxx.se/docs/copyright.html.
   *
   * You may opt to use, copy, modify, merge, publish, distribute and/or sell
   * copies of the Software, and permit persons to whom the Software is
   * furnished to do so, under the terms of the COPYING file.
   *
   * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
   * KIND, either express or implied.
   *
   **************************************************************************
   *
   * And the file COPYING mentioned above has the following contents:
   *
   * COPYRIGHT AND PERMISSION NOTICE
   *
   * Copyright (c) 1996 - 2017, Daniel Stenberg, <daniel@haxx.se>, and many
   * contributors, see the THANKS file.
   *
   * All rights reserved.
   *
   * Permission to use, copy, modify, and distribute this software for any purpose
   * with or without fee is hereby granted, provided that the above copyright
   * notice and this permission notice appear in all copies.
   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
   * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
   * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
   * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
   * OR OTHER DEALINGS IN THE SOFTWARE.
   *
   * Except as contained in this notice, the name of a copyright holder shall not
   * be used in advertising or otherwise to promote the sale, use or other dealings
   * in this Software without prior written authorization of the copyright holder.
   *
   */

  basic_Error * e = (basic_Error*)parm;

  for (std::string pem : cacerts::pems) {
    /* TODO: Currently this function does not report partial failure
     *       since we are adding all CA certs as described at
     *       https://curl.haxx.se/docs/caextract.html
     *
     *       The vast majority of these certificates are in fact not
     *       needed to authenticate cryptolens.io, and a failure
     *       adding most certificates is not a fatal error.
     */
    BIO * bio = BIO_new_mem_buf((void *)pem.c_str(), -1);
    X509 *cert = nullptr;

    PEM_read_bio_X509(bio, &cert, 0, NULL);
    if (cert == nullptr) {
      BIO_free(bio);
      continue;
    }

    X509_STORE * store = SSL_CTX_get_cert_store((SSL_CTX *)sslctx);

    if (X509_STORE_add_cert(store, cert) == 0) { /* TODO: Could add error handling */ }

    X509_free(cert);
    BIO_free(bio);
  }

  return CURLE_OK;
}

#endif /* CRYPTOLENS_CURL_EMBED_CACERTS */

RequestHandler_curl::RequestHandler_curl()
{
  this->curl = curl_easy_init();
}

std::string
RequestHandler_curl::make_request_(basic_Error & e, std::string const& url, std::string const& postfields)
{
  if (e) { return ""; }

  using namespace errors;
  using namespace errors::RequestHandler_curl;
  api::main api;

  if (!this->curl) { e.set(api, Subsystem::RequestHandler, CURL_NULL); return ""; }

  std::string response;
  CURLcode cc;

  cc = curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  if (cc != CURLE_OK) { e.set(api, Subsystem::RequestHandler, SETOPT_URL, cc); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, handle_response);
  if (cc != CURLE_OK) { e.set(api, Subsystem::RequestHandler, SETOPT_WRITEFUNCTION, cc); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&response);
  if (cc != CURLE_OK) { e.set(api, Subsystem::RequestHandler, SETOPT_WRITEDATA, cc); return ""; }
  cc = curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, postfields.c_str());
  if (cc != CURLE_OK) { e.set(api, Subsystem::RequestHandler, SETOPT_POSTFIELDS, cc); return ""; }

#ifdef CRYPTOLENS_CURL_EMBED_CACERTS
  curl_easy_setopt(this->curl, CURLOPT_SSL_CTX_FUNCTION, *sslctx_function_setup_cacerts);
  curl_easy_setopt(this->curl, CURLOPT_SSL_CTX_DATA, (void*)&e);
#endif /* CRYPTOLENS_CURL_EMBED_CACERTS */

  cc = curl_easy_perform(this->curl);
  if (cc != CURLE_OK) { e.set(api, Subsystem::RequestHandler, PERFORM, cc); return ""; }

  return response;
}

RequestHandler_curl::~RequestHandler_curl()
{
  if (this->curl) {
    curl_easy_cleanup(this->curl);
  }
}

} // namespace v20190401

} // namespace cryptolens_io
