#include <cryptolens_cli.hpp>

using namespace System;
using namespace System::IO;

int main(array<System::String^>^ args)
{
	int exit_code = 0;

	Console::WriteLine("Trying to activate license.");

	cryptolens_io::cli::CryptolensResult cryptolens_result = cryptolens_io::cli::cryptolens_perform("MPDWY-PQAOW-FKSCH-SGAAU");

	if (cryptolens_result.subsystem != 0) {
		Console::WriteLine("Failed to activate license.");
		exit_code = 1;
		goto cleanup;
	}

	Console::WriteLine("Status: subsystem={0} reason={1} extra={2}", cryptolens_result.subsystem, cryptolens_result.reason, cryptolens_result.extra);

	System::String^ key = cryptolens_result.key != nullptr ? gcnew System::String(cryptolens_result.key) : gcnew System::String("Unknown");

	Console::WriteLine("Activated key: {0}", key);

cleanup:
	cryptolens_io::cli::cryptolens_destroy(&cryptolens_result);

	return exit_code;
}
