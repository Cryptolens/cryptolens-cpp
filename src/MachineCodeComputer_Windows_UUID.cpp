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

size_t SHA256(basic_Error& e, char* data, size_t n, std::string& append)
{
        api::main api;

	// Initialization from https://devblogs.microsoft.com/oldnewthing/20160127-00/?p=92934
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	BYTE* pbHash = NULL;
	DWORD dwHashLen;
	DWORD dwHashLenSize = sizeof(DWORD);

	CHAR HEX[] = "0123456789ABCDEF";

	size_t result = 1;

	if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		e.set(api, 6, 15, result); goto cleanup;
	}

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
	{
		e.set(api, 6, 16, result); goto cleanup;
	}

	if (n > 0xFFFFFFFF) { e.set(api, 6, 30, result); goto cleanup; }

	if (!CryptHashData(hHash, (const BYTE*)data, (DWORD)n, 0))
	{
		e.set(api, 6, 17, result); goto cleanup;
	}

	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&dwHashLen, &dwHashLenSize, 0))
	{
		e.set(api, 6, 18, result); goto cleanup;
	}

	if (!(pbHash = (BYTE*)malloc(dwHashLen)))
	{
		e.set(api, 6, 19, result); goto cleanup;
	}

	if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))
	{
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
	if (hHash) { CryptDestroyHash(hHash); }
	if (hProv) { CryptReleaseContext(hProv, 0); }
	free(pbHash);

	return result;
}

} // namespace internal

std::string
MachineCodeComputer_Windows_UUID_Recompute::get_machine_code(basic_Error & e) {
        std::string hashed_machine_code;
        int s = INITIAL_BUFFER_SIZE;
        int p = 0;
        BOOL r = 0;
        DWORD c = 0;
        char* buffer = NULL;
        char* b = NULL;
        HANDLE pipe_read = INVALID_HANDLE_VALUE;
        HANDLE pipe_write = INVALID_HANDLE_VALUE;
        SECURITY_ATTRIBUTES sa;
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        wchar_t command[] = L"cmd.exe /c powershell.exe -Command \"(Get-CimInstance -Class Win32_ComputerSystemProduct).UUID\"";

        buffer = (char*)malloc(s * sizeof(char));
        if (!buffer) { goto error; }

        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = true;
        sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&pipe_read, &pipe_write, &sa, 0)) { goto error; }
        if (!SetHandleInformation(pipe_read, HANDLE_FLAG_INHERIT, 0)) { goto error; }

        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdOutput = pipe_write;
        si.dwFlags |= STARTF_USESTDHANDLES;

        r = CreateProcess(
                NULL,
                command,
                NULL,
                NULL,
                true,
                CREATE_NO_WINDOW,
                NULL,
                NULL,
                &si,
                &pi
        );

        CloseHandle(pipe_write);

        if (!r) { goto error; }

	for (;;) {
            if (p + CHUNK_SIZE < s) {
                s *= 2;

                b = (char*)realloc(buffer, s * sizeof(char));
                if (b == NULL) { goto error; }

                buffer = b;
            }

            r = ReadFile(pipe_read, buffer + p, CHUNK_SIZE, &c, NULL);

            if (!r && c != 0) { goto error; }

            p += c;
            if (c == 0) { break; }
        }

        ::cryptolens_io::v20190401::internal::SHA256(e, buffer, p, hashed_machine_code);

	goto exit;
error:
        e.set(api::main(), 6, 1);
exit:
        CloseHandle(pipe_read);
        free(buffer);

        return hashed_machine_code;
}

} // namespace v20190401

} // namespace cryptolens_io
