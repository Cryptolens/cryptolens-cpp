#include <iostream>

#include <curl/curl.h>

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_Unix.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;

// Set up an alias for the handler class used to interact with the library. We use Configuration_Unix_IgnoreExpires which does not checks the expiry date
// on the license automatically against the system time. We also provide Configuration_Unix_CheckExpires which automatically performs such a check.
//
// For this example we use MachineCodeComputer_static where the machine code is set using set_machine_code(). Other
// ways of computing the machine code are available, see README.md.
using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Unix_IgnoreExpires<cryptolens::MachineCodeComputer_static>>;

/*
 * This example uses the basic_Cryptolens class to first create a trial key and
 * later performing an activation for this key.
 */
int main()
{
  curl_global_init(CURL_GLOBAL_SSL);

  cryptolens::Error e;
  Cryptolens cryptolens_handle(e);

  // Setting up the cryptographic keys needed to verify the server response.
  // These values are different for different accounts and can be found under "Security Settings" on cryptolens.io.
  cryptolens_handle.signature_verifier.set_public_key_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==", "AQAB");

  // This line is only for MachineCodeComputer_static and sets the machine code to a static value
  cryptolens_handle.machine_code_computer.set_machine_code(e, "5bccbfb6567abdcf998b7c74190183ac315720a4fd4da56bac4f31be571bbb30");

  // We use two tokens, one for the call the activate() and one for the call to create_trial_key()
  // This is in order to allow different values for the Feature Lock field on the two tokens.
  // If you do not wish to override Feature Lock one token is sufficient.
  std::string activate_token         = "WyIzNTU2IiwiaXdGeUM1UVhXTGpiVjdtM0tYS3RVclFBMmt4RldpekMrSlJKcXRLeiJd";
  std::string create_trial_key_token = "WyIzNTU1IiwiRjFyOUZPZzRxUk5qanBXa005ZUtycWN0WGJEOVFpQTdtNjFzbEFPayJd";

  // The variable key_string is the license key string on the form "ABCD-1234-..."
  std::string key_string = cryptolens_handle.create_trial_key(e, create_trial_key_token, 4435);

  // Now we make an activate call with the new key_string to get the full LicenseKey object
  // with information about expiry dates and so on
  cryptolens::optional<cryptolens::LicenseKey> license_key =
    cryptolens_handle.activate(
      e, // Object used for reporting if an error occured
      activate_token, // Cryptolens Access Token
      4435, // Product id
      key_string // License Key
    );

  if (e) {
    // Error occured trying to activate the license key
    using namespace cryptolens::errors;

    if (e.get_subsystem() == Subsystem::Main) {
      // Handle errors from the Cryptolens API
      std::cout << "Cryptolens error: " << e.get_reason() << std::endl;
    } else if (e.get_subsystem() == Subsystem::RequestHandler && e.get_reason() == RequestHandler_curl::PERFORM) {
      int curlcode = e.get_extra();
      std::cout << "Error connecting to the server: curlcode: " << curlcode << std::endl;
    } else {
      std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
    }
    return 1;
  }

  // Use LicenseKeyChecker to check properties of the license key
  if (license_key->check().has_expired(1234567)) {
    // Above, the value 1234567 represents the current time as a unix timestamp in seconds
    std::cout << "Your subscription has expired." << std::endl;
    return 1;
  }

  if (license_key->check().has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
  else                                     { std::cout << "Welcome!" << std::endl; }

  curl_global_cleanup();
}
