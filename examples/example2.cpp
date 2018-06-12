#include <iostream>

#include "cryptolens/basic_SKM.hpp"
#include "cryptolens/Error.hpp"
#include "cryptolens/SignatureVerifier_OpenSSL.hpp"

namespace skm = serialkeymanager_com;

/*
 * This example assumes the reply from the web server has fetched in some
 * other fasion, and then verifies the signature and finally checks
 * some properties on the license key.
 */

int main()
{
  skm::Error e;
  skm::SignatureVerifier_OpenSSL signature_verifier;
  // Setting up the signature verifier with credentials from "Security Settings"
  // on serialkeymanager.com
  signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  signature_verifier.set_exponent_base64(e, "AQAB");

  // Reply from SKM
  std::string skm_reply("{\"licenseKey\":\"eyJQcm9kdWN0SWQiOjM2NDYsIklEIjo0LCJLZXkiOiJNUERXWS1QUUFPVy1GS1NDSC1TR0FBVSIsIkNyZWF0ZWQiOjE0OTAzMTM2MDAsIkV4cGlyZXMiOjE0OTI5MDU2MDAsIlBlcmlvZCI6MzAsIkYxIjpmYWxzZSwiRjIiOmZhbHNlLCJGMyI6ZmFsc2UsIkY0IjpmYWxzZSwiRjUiOmZhbHNlLCJGNiI6ZmFsc2UsIkY3IjpmYWxzZSwiRjgiOmZhbHNlLCJOb3RlcyI6bnVsbCwiQmxvY2siOmZhbHNlLCJHbG9iYWxJZCI6MzE4NzYsIkN1c3RvbWVyIjpudWxsLCJBY3RpdmF0ZWRNYWNoaW5lcyI6W3siTWlkIjoiIiwiSVAiOiIxNTUuNC4xMzQuMjciLCJUaW1lIjoxNDkxODk4OTE4fSx7Ik1pZCI6ImxvbCIsIklQIjoiMTU1LjQuMTM0LjI3IiwiVGltZSI6MTQ5MTg5ODk5NX0seyJNaWQiOiIyODlqZjJhZnNmIiwiSVAiOiIxNTUuNC4xMzQuMjciLCJUaW1lIjoxNDkxOTAwNTQ2fSx7Ik1pZCI6IjI4OWpmMmFmczMiLCJJUCI6IjE1NS40LjEzNC4yNyIsIlRpbWUiOjE0OTE5MDA2MzZ9XSwiVHJpYWxBY3RpdmF0aW9uIjpmYWxzZSwiTWF4Tm9PZk1hY2hpbmVzIjoxMCwiQWxsb3dlZE1hY2hpbmVzIjoiIiwiRGF0YU9iamVjdHMiOltdLCJTaWduRGF0ZSI6MTQ5NTAxOTc2Nn0=\",\"signature\":\"SqPm8dtTdVBrXrmJzXer7qq6dvdQfctJxP8mar+RO9p8QABsgWWaX+uH7aOGMBd42eg+2Omorv7Ks6V7itRhXPeeq5qWoKuefd+pTsFagvqiu2N/E2Np8fpt51aqmiygdHLECo42nJwVD8JzlN67hnvJTgY7iyDWhG7qFK9Slk+kEJjjK/0J1pJYI6nOi+7sgBV7ZRca+7DmiP6OmOjNfySps6PdiB7QbiSis5f24Xmc5OYyRe3fzZmAueqF3eymBK19XhYFroWXeT4tcNsBNJsv+YfItovGbJysLx+K4ppltd2GNwEFQgtE3ILGOUj7EVbeQmQXg9m2c5MTPyk8iA==\",\"result\":0,\"message\":\"\"}");

  skm::optional<skm::RawLicenseKey> rawLicenseKey = skm::handle_activate(e, signature_verifier, skm_reply);

  // Check if request was successful and signature is valid
  if (!rawLicenseKey) {
    std::cout << "Failed to construct raw license key!" << std::endl;
    return 1;
  }

  // Create the license key
  skm::optional<skm::LicenseKey> license_key = skm::LicenseKey::make(e, rawLicenseKey);

  if (!license_key) {
    std::cout << "Failed to construct license key!" << std::endl;
    std::cout << "This should never happen unless server does weird things." << std::endl;
    return 1;
  }

  std::cout << license_key->get_product_id() << std::endl;

  // Use LicenseKeyChecker to check properties of the license key
  if (license_key->check().has_feature(1)) {
    std::cout << "You Are Great And Super Customer!" << std::endl;
    std::cout << "Welcome!" << std::endl;
  }
}
