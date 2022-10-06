#include <iostream>

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>

#include <cryptolens/ResponseParser_ArduinoJson5.hpp>
#include <cryptolens/RequestHandler_static.hpp>
#include <cryptolens/SignatureVerifier_OpenSSL.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>

#include <cryptolens/validators/AndValidator.hpp>
#include <cryptolens/validators/CorrectKeyValidator.hpp>
#include <cryptolens/validators/CorrectProductValidator.hpp>
#include <cryptolens/validators/NotExpiredValidator_ctime.hpp>
#include <cryptolens/validators/OnValidMachineValidator.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;

struct CryptolensConfiguration {
  using ResponseParser = cryptolens::ResponseParser_ArduinoJson5;
  using RequestHandler = cryptolens::RequestHandler_static;
  using SignatureVerifier = cryptolens::SignatureVerifier_OpenSSL;
  using MachineCodeComputer = cryptolens::MachineCodeComputer_static;

  template<typename Env>
  using ActivateValidator = cryptolens::AndValidator_<Env, cryptolens::CorrectKeyValidator_<Env>
                          , cryptolens::AndValidator_<Env, cryptolens::CorrectProductValidator_<Env>
                          , cryptolens::AndValidator_<Env, cryptolens::OnValidMachineValidator_<Env>
                          ,                                cryptolens::NotExpiredValidator_ctime_<Env>
                          >>>;

  template<typename Env>
  using GetKeyValidator = cryptolens::AndValidator_<Env, cryptolens::CorrectKeyValidator_<Env>
                        , cryptolens::AndValidator_<Env, cryptolens::CorrectProductValidator_<Env>
                        >>;
};

using Cryptolens = cryptolens::basic_Cryptolens<CryptolensConfiguration>;

/*
 * This example uses the basic_Cryptolens class to make a request to the WebAPI
 * and then checks some properties of the license keys.
 */

int main()
{
  cryptolens::Error e;
  Cryptolens cryptolens_handle(e);
  // Setting up the signature verifier with credentials from "Security Settings"
  // on cryptolens.io
  cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  cryptolens_handle.signature_verifier.set_exponent_base64(e, "AQAB");

  cryptolens_handle.machine_code_computer.set_machine_code(e, "289jf2afs3");
  cryptolens_handle.request_handler.set_response("{\"licenseKey\":\"eyJQcm9kdWN0SWQiOjM2NDYsIklEIjo0LCJLZXkiOiJNUERXWS1QUUFPVy1GS1NDSC1TR0FBVSIsIkNyZWF0ZWQiOjE0OTAzMTM2MDAsIkV4cGlyZXMiOjI0MDA3NTU1OTUsIlBlcmlvZCI6MTAwMDAsIkYxIjpmYWxzZSwiRjIiOmZhbHNlLCJGMyI6ZmFsc2UsIkY0IjpmYWxzZSwiRjUiOmZhbHNlLCJGNiI6ZmFsc2UsIkY3IjpmYWxzZSwiRjgiOmZhbHNlLCJOb3RlcyI6bnVsbCwiQmxvY2siOmZhbHNlLCJHbG9iYWxJZCI6MzE4NzYsIkN1c3RvbWVyIjpudWxsLCJBY3RpdmF0ZWRNYWNoaW5lcyI6W3siTWlkIjoiMjg5amYyYWZzMyIsIklQIjoiMTU4LjE3NC4xODYuNDgiLCJUaW1lIjoxNjEyOTY2MDc2fSx7Ik1pZCI6InRlc3QxMjMiLCJJUCI6IjE1OC4xNzQuMjMuMjI3IiwiVGltZSI6MTY2MTg0MjExOH1dLCJUcmlhbEFjdGl2YXRpb24iOmZhbHNlLCJNYXhOb09mTWFjaGluZXMiOjIsIkFsbG93ZWRNYWNoaW5lcyI6IiIsIkRhdGFPYmplY3RzIjpbXSwiU2lnbkRhdGUiOjE2NjUwNTU5MDB9\",\"signature\":\"HZCqGvxREGAl6sGCzCeJ9wsSTf0CZHM5i1k9p1yUSmJ22o5DMajMZBGrQi3KvQli1RWYDY8dsGxwhH92oqkTsr9de4qHCGIdCo3ruEHtd6AfTBVyJhoYZezq/FQfVSdmXsYzIhhRmpdqUFDSeqXm/UoqXMTNx7akHfocr5YqjK8XOoyJPGMmvwNU73N0k71rD56+paShY6AeCU34Rx8bvmSqm6qkcxtOxxcg267QewcezO/V+QankGK012nr6AXFd3X8zvqEgsrMRjtEAxuKz2GhDMWscpc3WZgSIXpjqZ2XzQ0g/wZLqz5n5Qs8TlFkoZnzWHgH5VtKXhxdfKcrhQ==\",\"result\":0,\"message\":\"\"}");

  cryptolens::optional<cryptolens::LicenseKey> license_key =
    cryptolens_handle.activate
      ( // Object used for reporting if an error occured
        e
      , // Cryptolens Access Token
        "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0="
      , // Product id
        3646
      , // License Key
        "MPDWY-PQAOW-FKSCH-SGAAU"
      );

  std::string message = cryptolens_handle.last_message(e, "WyIxNzk0IiwiUkhSTWl3ekVBYTRlNjZFQUtVRUNObzBNWDAvZnRmQ2tKUGVkQWZJcSJd", "stable", 1234);

  if (e) {
    // Error occured trying to activate the license key
    using namespace cryptolens::errors;

    if (e.get_subsystem() == Subsystem::Main) {
      // Handle errors from the Cryptolens API
      std::cout << "Cryptolens error: " << e.get_reason() << std::endl;
    } else {
      std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
    }
    return 1;
  }

  std::cout << "MOTD: " << message << std::endl;
  std::cout << "License key for product with id: " << license_key->get_product_id() << std::endl;

  // Use LicenseKeyChecker to check properties of the license key
  if (license_key->check().has_expired(1234567)) {
    std::cout << "Your subscription has expired." << std::endl;
    return 1;
  }

  if (license_key->check().has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
  else                                     { std::cout << "Welcome!" << std::endl; }
}
