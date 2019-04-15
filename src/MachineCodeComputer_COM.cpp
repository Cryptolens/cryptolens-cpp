#include <string>

# pragma comment(lib, "wbemuuid.lib")
#define _WIN32_DCOM
#include "imports/windows/atlconv.h"
#include "imports/windows/atlsecurity.h"
#include "imports/windows/comdef.h"
#include "imports/windows/iphlpapi.h"
#include "imports/windows/Wbemidl.h"
#include "imports/windows/wincrypt.h"
#include "imports/windows/Windows.h"

#include "api.hpp"
#include "basic_Error.hpp"
#include "MachineCodeComputer_COM.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

api::main api;

HRESULT GetWbemService(basic_Error & e, IWbemLocator** pLocator, IWbemServices** pService)
{
	HRESULT result;

	result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(pLocator));
	if (FAILED(result)) { e.set(api, 6, 3, result); goto cleanup; }

	result = (*pLocator)->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, pService);
	if (FAILED(result)) { e.set(api, 6, 4, result); goto cleanup; }

	result = CoSetProxyBlanket(*pService, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(result)) { e.set(api, 6, 5, result); goto cleanup; }

	return NOERROR;

cleanup:
	if (pService && *pService) { (*pService)->Release(); }
	if (pLocator && *pLocator) { (*pLocator)->Release(); }
	CoUninitialize();

	return result;
}

HRESULT QueryAllBstr(basic_Error & e, IWbemServices* pService, const wchar_t* query, const wchar_t* propertyName, std::wstring & append)
{
	IEnumWbemClassObject* pEnumerator = NULL;
	IWbemClassObject *pQueryObject = NULL;

	HRESULT result = pService->ExecQuery(
		bstr_t(L"WQL"),                                         // strQueryLanguage
		bstr_t(query),                                          // strQuery
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,  // lFlags
		NULL,                                                   // pCtx
		&pEnumerator                                            // ppEnum
	);
	if (FAILED(result)) { e.set(api, 6, 6, result); goto cleanup; }

	while (pEnumerator)
	{
		try
		{
			ULONG returnedObjectCount = 0;
			result = pEnumerator->Next(WBEM_INFINITE, 1, &pQueryObject, &returnedObjectCount);

			if (returnedObjectCount == 0) { break; }

			VARIANT objectProperty;
			result = pQueryObject->Get(propertyName, 0, &objectProperty, 0, 0);
			if (FAILED(result)) { e.set(api, 6, 7, result); goto cleanup; }

			if (objectProperty.vt == VT_BSTR)
			{
				append += objectProperty.bstrVal;
			}

			VariantClear(&objectProperty);
		}
		catch (...)
		{
			break;
		}
	}

	result = NOERROR;

cleanup:
	if (pEnumerator) { pEnumerator->Release(); }
	if (pQueryObject) { pQueryObject->Release(); }

	return result;
}

HRESULT QueryFirstBstr(basic_Error & e, IWbemServices* pService, const wchar_t* query, const wchar_t* propertyName, std::wstring & append)
{
	IEnumWbemClassObject* pEnumerator = NULL;
	IWbemClassObject *pQueryObject = NULL;

	HRESULT result = pService->ExecQuery(
		bstr_t(L"WQL"),                                         // strQueryLanguage
		bstr_t(query),                                          // strQuery
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,  // lFlags
		NULL,                                                   // pCtx
		&pEnumerator                                            // ppEnum
	);
	if (FAILED(result)) { e.set(api, 6, 8, result); goto cleanup; }

	while (pEnumerator)
	{
		try
		{
			ULONG returnedObjectCount = 0;
			result = pEnumerator->Next(WBEM_INFINITE, 1, &pQueryObject, &returnedObjectCount);

			if (returnedObjectCount == 0) { break; }

			VARIANT objectProperty;
			result = pQueryObject->Get(propertyName, 0, &objectProperty, 0, 0);
			if (FAILED(result)) { e.set(api, 6, 9, result); goto cleanup; }

			if (objectProperty.vt == VT_BSTR)
			{
				append += objectProperty.bstrVal;
				break;
			}

			VariantClear(&objectProperty);
		}
		catch (...)
		{
			break;
		}
	}

	result = NOERROR;

cleanup:
	if (pEnumerator) { pEnumerator->Release(); }
	if (pQueryObject) { pQueryObject->Release(); }

	return result;
}

HRESULT QueryFirstI4(basic_Error & e, IWbemServices* pService, const wchar_t* query, const wchar_t* propertyName, SHORT & append)
{
	IEnumWbemClassObject* pEnumerator = NULL;
	IWbemClassObject *pQueryObject = NULL;

	HRESULT result = pService->ExecQuery(
		bstr_t(L"WQL"),                                         // strQueryLanguage
		bstr_t(query),                                          // strQuery
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,  // lFlags
		NULL,                                                   // pCtx
		&pEnumerator                                            // ppEnum
	);
	if (FAILED(result)) { e.set(api, 6, 10, result); goto cleanup; }

	while (pEnumerator)
	{
		try
		{
			ULONG returnedObjectCount = 0;
			result = pEnumerator->Next(WBEM_INFINITE, 1, &pQueryObject, &returnedObjectCount);

			if (returnedObjectCount == 0) { break; }

			VARIANT objectProperty;
			result = pQueryObject->Get(propertyName, 0, &objectProperty, 0, 0);
			if (FAILED(result)) { e.set(api, 6, 11, result); goto cleanup; }

			if (objectProperty.vt == VT_I4)
			{
				append = objectProperty.iVal;
				break;
			}

			VariantClear(&objectProperty);
		}
		catch (...)
		{
			break;
		}
	}

	result = NOERROR;

cleanup:
	if (pEnumerator) { pEnumerator->Release(); }
	if (pQueryObject) { pQueryObject->Release(); }

	return result;
}

HRESULT GetHDDInfo(basic_Error & e, IWbemServices* pService, std::wstring & append)
{
	HRESULT result = NOERROR;

	SHORT diskindex = 999;
	_bstr_t query_devicename(L"SELECT * FROM Win32_DiskDrive where Index = ");
	std::wstring diskindex_str;
	std::wstring devicename;
	_bstr_t query_serial(L"SELECT * FROM Win32_PhysicalMedia WHERE Tag like '");

	bool nonwhitespacefound;

	result = QueryFirstI4(e, pService, L"select * from Win32_DiskPartition WHERE BootPartition=True", L"DiskIndex", diskindex);
	if (FAILED(result)) { e.set(api, 6, 12, result); goto cleanup; }
	if (diskindex == 999) { goto cleanup; } // Mirrors the dotnet code

	diskindex_str = std::to_wstring(diskindex);
	query_devicename += diskindex_str.c_str();
	result = QueryFirstBstr(e, pService, query_devicename.GetBSTR(), L"Name", devicename);
	if (FAILED(result)) { e.set(api, 6, 13, result); goto cleanup; }

	nonwhitespacefound = false;
	for (auto x : devicename)
	{
		// XXX: Maybe change to _iswspace_l and make it independent of current locale? Otoh dotnet might not do this?
		if (iswspace(x) == 0) { nonwhitespacefound = true; }
	}
	if (!nonwhitespacefound) { goto cleanup; }

	// XXX: This assumes \\.\ only occurs once. Can use find() to make it more similar to dotnet?
	if (devicename.size() > 4 && devicename[0] == L'\\' && devicename[1] == L'\\' && devicename[2] == L'.' && devicename[3] == L'\\')
	{
		query_serial += L"%";
		query_serial += devicename.c_str() + 4;
		query_serial += L"'";
	}
	else
	{
		query_serial += devicename.c_str();
	}
	result = QueryFirstBstr(e, pService, query_serial.GetBSTR(), L"SerialNumber", append);
	if (FAILED(result)) { e.set(api, 6, 14, result); goto cleanup; }

	result = NOERROR;

cleanup:
	return result;
}

HRESULT GetNICInfo(basic_Error & e, std::wstring & append)
{
	WCHAR HEX[] = L"0123456789ABCDEF";

	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;
	IP_ADAPTER_INFO* pAdapterInfos = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));

	// retry up to 5 times, to get the adapter infos needed
	for (int i = 0; i < 5 && (dwRetVal == ERROR_BUFFER_OVERFLOW || dwRetVal == NO_ERROR); ++i)
	{
		dwRetVal = GetAdaptersInfo(pAdapterInfos, &outBufLen);
		if (dwRetVal == NO_ERROR)
		{
			break;
		}
		else if (dwRetVal == ERROR_BUFFER_OVERFLOW)
		{
			free(pAdapterInfos);
			pAdapterInfos = (IP_ADAPTER_INFO*)malloc(outBufLen);
		}
		else
		{
			pAdapterInfos = 0;
			break;
		}
		//std::cout << i << std::endl;
	}
	if (dwRetVal == NO_ERROR)
	{
		IP_ADAPTER_INFO* pAdapterInfo = pAdapterInfos;
		while (pAdapterInfo)
		{
			IP_ADDR_STRING* pIpAddress = &(pAdapterInfo->IpAddressList);
			while (pIpAddress != 0)
			{
				if (pAdapterInfo->AddressLength > 0)
				{
					for (size_t i = 0; i < pAdapterInfo->AddressLength; ++i)
					{
						BYTE x = pAdapterInfo->Address[i];
						int x1 = (x & 0xF0) >> 4;
						int x2 = (x & 0xF) >> 0;
						append += HEX[x1];
						append += HEX[x2];
					}

					break;
				}

				pIpAddress = pIpAddress->Next;
			}
			pAdapterInfo = pAdapterInfo->Next;
		}
	}
	free(pAdapterInfos);

	return NOERROR;
}

HRESULT GetWindowsIdentity(basic_Error & e, std::wstring & append)
{
	ATL::CAccessToken accessToken;
	ATL::CSid currentUserSid;
	if (accessToken.GetProcessToken(TOKEN_READ | TOKEN_QUERY)
		&& accessToken.GetUser(&currentUserSid))
	{
		append += currentUserSid.Domain();
		append += L"\\";
		append += currentUserSid.AccountName();
	}

	return NOERROR;
}

size_t SHA1(basic_Error & e, std::wstring const& data, std::string & append)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	BYTE *pbHash;
	DWORD dwHashLen;
	DWORD dwHashLenSize = sizeof(DWORD);
	size_t data_len;

	CHAR HEX[] = "0123456789ABCDEF";

	size_t result;

	if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		result = 1;
		e.set(api, 6, 15, result); goto cleanup;
	}

	if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
	{
		result = 1;
		e.set(api, 6, 16, result); goto cleanup;
	}

	data_len = sizeof(wchar_t) * data.size();
	if (data_len > 0xFFFFFFFF) { result = 1; e.set(api, 6, 30, result); goto cleanup; }

	if (!CryptHashData(hHash, (const BYTE*)data.data(), (DWORD)data_len, 0))
	{
		result = 1;
		e.set(api, 6, 17, result); goto cleanup;
	}

	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen, &dwHashLenSize, 0))
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

	return result;
}

void AppendTemp(std::wstring & machinecode, std::wstring & machinecode_debug, std::wstring &  temp)
{
	machinecode += temp;
	machinecode_debug += temp; machinecode_debug += L",";
	temp.clear();
}

void GetMachineCode(basic_Error & e, std::wstring & append, std::wstring & append_debug)
{
	std::wstring temp;
	std::wstring machinecode_debug;
	std::wstring machinecode;

	 api::main api;

	HRESULT result, result_init;
	IWbemLocator* pLocator = NULL;
	IWbemServices* pService = NULL;

	result_init = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (result_init == RPC_E_CHANGED_MODE) {
		result_init = CoInitializeEx(0, COINIT_MULTITHREADED);
	}
	if (FAILED(result_init)) { e.set(api, 6, 1, result_init); goto cleanup; }

	result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(result) && result != RPC_E_TOO_LATE) { e.set(api, 6, 2, result); goto cleanup; }
	result = GetWbemService(e, &pLocator, &pService);
	if (FAILED(result)) { goto cleanup; }


	result = QueryAllBstr(e, pService, L"SELECT ProcessorId FROM Win32_Processor", L"ProcessorId", temp);
	if (FAILED(result)) { goto cleanup; }
	AppendTemp(machinecode, machinecode_debug, temp);

	result = QueryAllBstr(e, pService, L"select * from Win32_BIOS", L"SerialNumber", temp);
	if (FAILED(result)) { goto cleanup; }
	AppendTemp(machinecode, machinecode_debug, temp);

	result = QueryAllBstr(e, pService, L"select * from Win32_BaseBoard", L"SerialNumber", temp);
	if (FAILED(result)) { goto cleanup; }
	AppendTemp(machinecode, machinecode_debug, temp);

	if (machinecode.size() <= 3)
	{
		result = GetHDDInfo(e, pService, temp);
		if (FAILED(result)) { goto cleanup; }
		AppendTemp(machinecode, machinecode_debug, temp);
	}

	if (machinecode.find(L"To be filled by O.E.M.", 0) != std::wstring::npos)
	{
		result = GetNICInfo(e, temp);
		if (FAILED(result)) { goto cleanup; }
		AppendTemp(machinecode, machinecode_debug, temp);
	}

	if (false)
	{
		result = GetWindowsIdentity(e, temp);
		if (FAILED(result)) { goto cleanup; }
		AppendTemp(machinecode, machinecode_debug, temp);
	}

cleanup:
	if (pService) { pService->Release(); }
	if (pLocator) { pLocator->Release(); }
	if (!FAILED(result_init)) { CoUninitialize(); } // See some msdn article

	//return machinecode;

	append += machinecode;
	append_debug += machinecode_debug;
}

} // namespace internal

std::string
MachineCodeComputer_COM_Recompute::get_machine_code(basic_Error & e) {
	std::wstring machine_code;
	std::wstring debug;

	::cryptolens_io::v20190401::internal::GetMachineCode(e, machine_code, debug);

	std::string hashed_machine_code;
	::cryptolens_io::v20190401::internal::SHA1(e, machine_code, hashed_machine_code);

	return hashed_machine_code;
}

} // namespace v20190401

} // namespace cryptolens_io
