cmake_minimum_required (VERSION 3.0.2)
project (cryptolens)

set (CRYPTOLENS_CURL_EMBED_CACERTS OFF CACHE BOOL "embed the ca certs in the library instead of using curl defaults?")

set (SRC  "../../../src/ActivateError.cpp" "../../../src/DataObject.cpp" "../../../src/LicenseKey.cpp" "../../../src/LicenseKeyChecker.cpp" "../../../src/LicenseKeyInformation.cpp" "../../../src/base64.cpp" "../../../src/basic_SKM.cpp")
set (SRC ${SRC} "../../../src/SignatureVerifier_OpenSSL.cpp")
set (SRC ${SRC} "../../../src/RequestHandler_curl.cpp")

if (${CRYPTOLENS_CURL_EMBED_CACERTS})
  add_definitions (-DCRYPTOLENS_CURL_EMBED_CACERTS)
  set (SRC ${SRC} "../../../src/RequestHandler_curl_cacerts.cpp")
endif ()

add_library (cryptolens STATIC ${SRC})

target_link_libraries (cryptolens dl)
target_link_libraries (cryptolens pthread)
target_link_libraries (cryptolens "/home/builder/curl/install/lib/libcurl.a" "/home/builder/ssl/install/lib/libssl.a" "/home/builder/ssl/install/lib/libcrypto.a")
target_include_directories (cryptolens PUBLIC "/home/builder/ssl/install/include")
target_include_directories (cryptolens PUBLIC "/home/builder/curl/install/include")

target_include_directories (cryptolens PRIVATE "../../../include/cryptolens")
target_include_directories (cryptolens PUBLIC "../../../include")

set_property (TARGET cryptolens PROPERTY CXX_STANDARD 11)
set_property (TARGET cryptolens PROPERTY CXX_STANDARD_REQURED ON)
