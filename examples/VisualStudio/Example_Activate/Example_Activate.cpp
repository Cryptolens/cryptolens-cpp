#include <iostream>

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_Windows.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;

// Set up an alias for the handler class used to interact with the library. This configuration checks the expiry date
// on the license. Use Configuration_Windows_IgnoreExpires to ignore the expiry date.
//
// For this example we use MachineCodeComputer_static where the machine code is set using set_machine_code(). Other
// ways of computing the machine code are available, see README.md.
using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Windows<cryptolens::MachineCodeComputer_static>>;

/*
* This example uses the basic_SKM class to make a request to the WebAPI
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

        // This line is only for MachineCodeComputer_static and sets the machine code to a static value
	cryptolens_handle.machine_code_computer.set_machine_code(e, "289jf2afs3");

	cryptolens::optional<cryptolens::LicenseKey> license_key =
		cryptolens_handle.activate(
			e,
			// SKM Access Token
			"WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0=",
			// Product id
			3646,
			// License Key
			"MPDWY-PQAOW-FKSCH-SGAAU"
		);

	if (e) {
		using namespace cryptolens::errors;

		if (e.get_subsystem() == Subsystem::Main) {
			// Handle errors from the SKM API
			std::cout << "SKM error: " << e.get_reason() << std::endl;
		}
		else if (e.get_subsystem() == Subsystem::RequestHandler) {
			size_t code = e.get_extra();
			std::cout << "Error connecting to the server: error code: " << code << std::endl;
		}
		else {
			std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
		}
		return 1;
	}

	std::cout << "License key for product with id: " << license_key->get_product_id() << std::endl;

	// Use LicenseKeyChecker to check properties of the license key
	if (license_key->check().has_expired(1234567)) {
		std::cout << "Your subscription has expired." << std::endl;
		return 1;
	}

	if (license_key->check().has_feature(1)) { std::cout << "Welcome! Pro version enabled!" << std::endl; }
	else { std::cout << "Welcome!" << std::endl; }

	std::string EXIT; std::getline(std::cin, EXIT);
}
