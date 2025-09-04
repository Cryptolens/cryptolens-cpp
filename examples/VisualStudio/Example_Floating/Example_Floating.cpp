#include <iostream>

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_Windows.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;

// Set up an alias for the handler class used to interact with the library. We use Configuration_Windows_IgnoreExpires which does not checks the expiry date
// on the license automatically against the system time. We also provide Configuration_Windows_CheckExpires which automatically performs such a check.
//
// For this example we use MachineCodeComputer_static where the machine code is set using set_machine_code(). Other
// ways of computing the machine code are available, see README.md.
using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Windows_IgnoreExpires<cryptolens::MachineCodeComputer_static>>;

/*
 * This example uses the basic_Cryptolens class to make a request to the WebAPI
 * and then checks some properties of the license keys.
 */
void activate_device(char const* machine_code)
{
  cryptolens::Error e;
  Cryptolens cryptolens_handle(e);

  // Setting up the cryptographic keys needed to verify the server response.
  // These values are different for different accounts and can be found under "Security Settings" on cryptolens.io.
  cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  cryptolens_handle.signature_verifier.set_exponent_base64(e, "AQAB");

  // This line is only for MachineCodeComputer_static and sets the machine code to a static value
  cryptolens_handle.machine_code_computer.set_machine_code(e, machine_code);

  cryptolens::optional<cryptolens::LicenseKey> license_key =
    cryptolens_handle.activate_floating(
      e, // Object used for reporting if an error occured
      "WyIxNDQzIiwiZXBCVGc1S2tOVmRielVibXAxaWdtdVNtOXlGTnR0ekhkN2lwRnVKQiJd", // Cryptolens Access Token
      3934, // Product id
      "KRQNS-CSPNV-XRQBT-MTSBI", // License Key
      300 // Floating interval
    );

  if (e) {
    // Error occured trying to activate the license key
    using namespace cryptolens::errors;

    if (e.get_subsystem() == Subsystem::Main && e.get_reason() == Main::DEVICE_LIMIT_REACHED) {
      std::cout << "Failed to activate device with machine code " << machine_code << ": Too many devices already activated" << std::endl;
    } else if (e.get_subsystem() == Subsystem::Main) {
      // Handle errors from the Cryptolens API
      std::cout << "Cryptolens error: " << e.get_reason() << std::endl;
    } else if (e.get_subsystem() == Subsystem::RequestHandler) {
      size_t code = e.get_extra();
      std::cout << "Error connecting to the server: error code: " << code << std::endl;
    } else {
      std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
    }
    return;
  }

  std::cout << "Successfully activated device with machine code " << machine_code << std::endl;
}

int main()
{
  std::cout << "This examples tried to perform floating activations in quick succession on a key where" << std::endl
            << " maxnoofmachines is set to 1, and thus only one concurrent use of the key is permited." << std::endl
            << std::endl
            << "We attempt to activate 9 different devices, but we only expect the first activation to" << std::endl
            << "be successful" << std::endl
            << std::endl;

  activate_device("a");
  activate_device("b");
  activate_device("c");
  activate_device("d");
  activate_device("e");
  activate_device("f");
  activate_device("g");
  activate_device("h");
  activate_device("i");

  std::string DUMMY; std::getline(std::cin, DUMMY);
}
