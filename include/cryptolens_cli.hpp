#pragma once

#include <cstdint>

namespace cryptolens_io {

namespace cli {

struct CryptolensResult
{
	std::uint64_t subsystem;
	std::uint64_t reason;
	std::uint64_t extra;

	char * key;
};

CryptolensResult __cdecl cryptolens_perform(char const* license_key_string);

void __cdecl cryptolens_destroy(CryptolensResult* result);

}

}