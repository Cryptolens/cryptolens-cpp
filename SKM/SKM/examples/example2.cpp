#include <iostream>

#include "basic_SKM.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

namespace skm = serialkeymanager_com;

int main()
{
  skm::SignatureVerifier_OpenSSL signature_verifier;
  signature_verifier.set_modulus_base64("khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  signature_verifier.set_exponent_base64("AQAB");

  std::string skm_reply("{\"licenseKey\":\"eyJQcm9kdWN0SWQiOjM2NDYsIklEIjo0LCJLZXkiOiJNUERXWS1QUUFPVy1GS1NDSC1TR0FBVSIsIkNyZWF0ZWQiOiIyMDE3LTAzLTI0VDAwOjAwOjAwIiwiRXhwaXJlcyI6IjIwMTctMDQtMjNUMDA6MDA6MDAiLCJQZXJpb2QiOjMwLCJGMSI6ZmFsc2UsIkYyIjpmYWxzZSwiRjMiOmZhbHNlLCJGNCI6ZmFsc2UsIkY1IjpmYWxzZSwiRjYiOmZhbHNlLCJGNyI6ZmFsc2UsIkY4IjpmYWxzZSwiTm90ZXMiOm51bGwsIkJsb2NrIjpmYWxzZSwiR2xvYmFsSWQiOjMxODc2LCJDdXN0b21lciI6bnVsbCwiQWN0aXZhdGVkTWFjaGluZXMiOlt7Ik1pZCI6IiIsIklQIjoiMTU1LjQuMTM0LjI3IiwiVGltZSI6IjIwMTctMDQtMTFUMDg6MjE6NTguNjU3In0seyJNaWQiOiJsb2wiLCJJUCI6IjE1NS40LjEzNC4yNyIsIlRpbWUiOiIyMDE3LTA0LTExVDA4OjIzOjE1LjI2NyJ9LHsiTWlkIjoiMjg5amYyYWZzZiIsIklQIjoiMTU1LjQuMTM0LjI3IiwiVGltZSI6IjIwMTctMDQtMTFUMDg6NDk6MDYuODY3In0seyJNaWQiOiIyODlqZjJhZnMzIiwiSVAiOiIxNTUuNC4xMzQuMjciLCJUaW1lIjoiMjAxNy0wNC0xMVQwODo1MDozNi4wOTMifV0sIlRyaWFsQWN0aXZhdGlvbiI6ZmFsc2UsIk1heE5vT2ZNYWNoaW5lcyI6MTAsIkFsbG93ZWRNYWNoaW5lcyI6IiIsIkRhdGFPYmplY3RzIjpbXSwiU2lnbkRhdGUiOiIyMDE3LTA0LTE5VDA5OjUzOjU3IiwiU2lnbmF0dXJlIjpudWxsfQ==\",\"signature\":\"dFwPJIyAMWT1OmyxmNIz6ySdqU50kR36EOJtindmU50qO1AHzDMQWzMM96spNpyqb8JL17sjF4t0baHzotn2UJfzZ4M++vFYJ7Dj7lmX1cbQZE4VpQoXrNQeKfO6NWDL7xvJyH1D8bAx9IRf3AQ3Hd7EMqkXnN8K4amEtXA83aVMShANGo1UcmLeEXZ5p6Z8cRaa00WzyP/sz6PLz4j9q9CnbgQnAROG+xTe0UelT3kCv+f7bbsDfKKfZLJirjUuh3MyTnLbI9qOvK9LgQEgIgrVhxouFzSHOizLfcM0wQITQMsKUvNgamFZE0YfHA5ujzK0u9/wys4+p7Zt5+jPPQ==\",\"result\":0,\"message\":\"\"}");

  skm::optional<skm::RawLicenseKey> rawLicenseKey = skm::handle_activate(signature_verifier, skm_reply);

  if (!rawLicenseKey) {
    std::cout << "Failed to construct raw license key!" << std::endl;
    return 1;
  }

  auto license_key = skm::LicenseKey::make(rawLicenseKey->get_license());

  std::cout << license_key->get_product_id() << std::endl;

  if (license_key->check().has_feature(1)) {
    std::cout << "lolz" << std::endl;
  }
}
