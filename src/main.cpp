#include "../include/cryptolens_cli.hpp"

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/Configuration_Windows.hpp>
#include <cryptolens/MachineCodeComputer_static.hpp>

namespace cryptolens = ::cryptolens_io::v20190401;

using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Windows<cryptolens::MachineCodeComputer_static>>;

namespace cryptolens_io {

namespace cli {

void cryptolens_result_init(CryptolensResult & result)
{
	result.subsystem = 0;
	result.reason = 0;
	result.extra = 0;

	result.key = nullptr;
}

namespace internal {

char* copy_string(std::string const& s)
{
	size_t n = s.size() + 1;
	char * c = new char[n];
	strcpy_s(c, n, s.c_str());

	return c;
}

char* copy_string(cryptolens::optional<std::string> const& o)
{
	char* c = nullptr;

	if (o) {
		c = copy_string(*o);
	}

	return c;
}

} // namespace internal

CryptolensResult __cdecl cryptolens_perform(char const* license_key_string)
{
	cryptolens::Error e;
	Cryptolens cryptolens_handle(e);
	CryptolensResult r;

	cryptolens_result_init(r);

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
			license_key_string
		);

	r.subsystem = e.get_subsystem();
	r.reason = e.get_reason();
	r.extra = e.get_extra();

	if (e) {
		return r;
	}

	r.key = internal::copy_string(license_key->get_key());

	return r;
}

void __cdecl cryptolens_destroy(CryptolensResult * result)
{
	delete[] result->key;
	result->key = nullptr;
}

} // namespace cli

} // namespace cryptolens_io