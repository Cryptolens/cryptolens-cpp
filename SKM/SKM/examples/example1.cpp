#include <iostream>

#include <curl/curl.h>

#include "basic_SKM.hpp"
#include "RequestHandler_curl.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

namespace skm = serialkeymanager_com;
using SKM = skm::basic_SKM<skm::RequestHandler_curl,skm::SignatureVerifier_OpenSSL>;

int main()
{
  curl_global_init(CURL_GLOBAL_SSL);

  SKM skm;
  skm.signature_verifier.set_modulus_base64("khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  skm.signature_verifier.set_exponent_base64("AQAB");

  skm::optional<skm::RawLicenseKey> rawLicenseKey =
    skm.activate
      ( "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0="
      , "3646"
      , "MPDWY-PQAOW-FKSCH-SGAAU"
      , "289jf2afs3"
      );

  if (!rawLicenseKey) {
    std::cout << "Failed to construct raw license key!" << std::endl;
    return 1;
  }

  auto license_key = skm::LicenseKey::make(rawLicenseKey->get_license());

  std::cout << license_key->get_product_id() << std::endl;

  if (license_key->check().has_feature(1)) {
    std::cout << "lolz" << std::endl;
  }

  curl_global_cleanup();
}
