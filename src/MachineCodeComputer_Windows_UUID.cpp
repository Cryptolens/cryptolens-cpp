#include <string>

#include "imports/windows/Windows.h"
#include "imports/windows/wincrypt.h"

#include "api.hpp"
#include "basic_Error.hpp"
#include "MachineCodeComputer_Windows_UUID.hpp"

#define CHUNK_SIZE 128
#define INITIAL_BUFFER_SIZE 1024

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

api::main api;

size_t SHA256(basic_Error& e, char* data, size_t n, std::string& append)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	BYTE* pbHash = NULL;
	DWORD dwHashLen;
	DWORD dwHashLenSize = sizeof(DWORD);

	CHAR HEX[] = "0123456789ABCDEF";

	size_t result;

	if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		result = 1;
		e.set(api, 6, 15, result); goto cleanup;
	}

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
	{
		result = 1;
		e.set(api, 6, 16, result); goto cleanup;
	}

	if (n > 0xFFFFFFFF) { result = 1; e.set(api, 6, 30, result); goto cleanup; }

	if (!CryptHashData(hHash, (const BYTE*)data, (DWORD)n, 0))
	{
		result = 1;
		e.set(api, 6, 17, result); goto cleanup;
	}

	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&dwHashLen, &dwHashLenSize, 0))
	{
		result = 1;
		e.set(api, 6, 18, result); goto cleanup;
	}

	if (!(pbHash = (BYTE*)malloc(dwHashLen)))
	{
		result = 1;
		e.set(api, 6, 19, result); goto cleanup;
	}

	if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))
	{
		result = 1;
		e.set(api, 6, 20, result); goto cleanup;
	}

	for (size_t i = 0; i < dwHashLen; ++i)
	{
		BYTE x = pbHash[i];
		int x1 = (x & 0xF0) >> 4;
		int x2 = (x & 0xF) >> 0;
		append += HEX[x1];
		append += HEX[x2];
	}

	result = 0;

cleanup:
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	free(pbHash);

	return result;
}

} // namespace internal

std::string
MachineCodeComputer_Windows_UUID_Recompute::get_machine_code(basic_Error & e) {
	std::string hashed_machine_code;

	int s = INITIAL_BUFFER_SIZE;
	int p = 0;
	int r = 0;
	char* buffer = NULL;
	char* b = NULL;
	FILE* pipe = NULL;


	pipe = _popen("cmd.exe /c powershell.exe -Command \"(Get-CimInstance -Class Win32_ComputerSystemProduct).UUID\"", "rb");
	if (pipe == NULL) {
		goto error;
	}

	buffer = (char*)malloc(s * sizeof(char));
	if (!buffer) { goto error; }

	for (;;) {
		if (p + CHUNK_SIZE < s) {
			s *= 2;

			b = (char*)realloc(buffer, s * sizeof(char));
			if (b == NULL) { goto error; }

			buffer = b;
		}

		r = fread(buffer + p, sizeof(char), CHUNK_SIZE, pipe);

		p += r;
		if (r < CHUNK_SIZE) { break; }
	}

	r = ferror(pipe);
	if (r) {
		goto error;
	}

	::cryptolens_io::v20190401::internal::SHA256(e, buffer, p, hashed_machine_code);

error:
exit:
	_pclose(pipe);
	free(buffer);

	return hashed_machine_code;
}

} // namespace v20190401

} // namespace cryptolens_io
