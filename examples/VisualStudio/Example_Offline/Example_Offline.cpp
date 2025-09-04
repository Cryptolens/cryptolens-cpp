#include <fstream>
#include <iostream>

#include <Windows.h>

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

int write_license_key_to_file(LPCWSTR);
int read_license_key_from_file(LPCWSTR);

/*
 * This is an example of doing offline activation.
 *
 * We start by performing an online activation, but store the response in a file.
 * Later, we read the file and check that the activation without making an additional
 * request to the Cryptolens Web API.
 */
int main()
{
  // For this example we store the license key in a temporary file
  UINT result;
  wchar_t temp_dir[MAX_PATH];
  wchar_t file_name[MAX_PATH];

  result = GetTempPath(MAX_PATH, temp_dir);
  if (result > MAX_PATH || result == 0) { std::cout << "Failed to get temporary directory" << std::endl; goto cleanup; }

  result = GetTempFileName(temp_dir, L"cryptolens", 0, file_name);
  if (result == 0) { std::cout << "Failed to create temporary file" << std::endl; goto cleanup; }

  write_license_key_to_file(file_name);
  read_license_key_from_file(file_name);

  std::wcout << std::endl << L"The saved response can be found in the file " << file_name << std::endl;

cleanup:
  std::string DUMMY; std::getline(std::cin, DUMMY);
  return 0;
}

int write_license_key_to_file(LPCWSTR file_name)
{
  /*
   * Standard code for performing online activation similar to example_activate.cpp
   */
  cryptolens::Error e;
  Cryptolens cryptolens_handle(e);

  // Setting up the cryptographic keys needed to verify the server response.
  // These values are different for different accounts and can be found under "Security Settings" on cryptolens.io.
  cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  cryptolens_handle.signature_verifier.set_exponent_base64(e, "AQAB");

  // This line is only for MachineCodeComputer_static and sets the machine code to a static value
  cryptolens_handle.machine_code_computer.set_machine_code(e, "5bccbfb6567abdcf998b7c74190183ac315720a4fd4da56bac4f31be571bbb30");

  cryptolens::optional<cryptolens::LicenseKey> license_key =
    cryptolens_handle.activate(
      e, // Object used for reporting if an error occured
      "WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0=", // Cryptolens Access Token
      3646, // Product id
      "MPDWY-PQAOW-FKSCH-SGAAU" // License Key
    );

  if (e) {
    // Error occured trying to activate the license key
    using namespace cryptolens::errors;

    if (e.get_subsystem() == Subsystem::Main) {
      // Handle errors from the Cryptolens API
      std::cout << "Cryptolens error: " << e.get_reason() << std::endl;
    } else if (e.get_subsystem() == Subsystem::RequestHandler) {
      size_t code = e.get_extra();
      std::cout << "Error connecting to the server: error code: " << code << std::endl;
    } else {
      std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
    }
    return 1;
  }

  /*
   * Offline specific part. Convert the response to string and save it in a file.
   */

  std::string s = license_key->to_string();

  std::ofstream out(file_name);
  out << s << std::endl;

  return 0;
}

int read_license_key_from_file(LPCWSTR file_name)
{
  /*
   * We need to set up the cryptolens_handle with the public keys as usual.
   */
  cryptolens::Error e;
  Cryptolens cryptolens_handle(e);

  // Setting up the cryptographic keys needed to verify the server response.
  // These values are different for different accounts and can be found under "Security Settings" on cryptolens.io.
  cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  cryptolens_handle.signature_verifier.set_exponent_base64(e, "AQAB");

  /*
   * Load the string and check that activation was performed correctly
   */

  std::string s;
  std::ifstream in(file_name);
  std::getline(in, s);

  cryptolens::optional<cryptolens::LicenseKey> license_key = cryptolens_handle.make_license_key(e, s);

  if (e) { std::cout << "Could not parse string" << std::endl; return 1; }

  /*
   * At this point we have checked the activation and can proceeed as usual
   */

  // Use LicenseKeyChecker to check properties of the license key
  if (license_key->check().has_expired(1234567)) {
    // Above, the value 1234567 represents the current time as a unix timestamp in seconds
    std::cout << "Your subscription has expired." << std::endl;
    return 1;
  }

  if (license_key->check().has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
  else                                     { std::cout << "Welcome!" << std::endl; }

  return 0;
}
