cmake_minimum_required (VERSION 3.0.2)
project (cryptolens)

set (CRYPTOLENS_BUILD_TESTS OFF CACHE BOOL "build tests?")
set (CRYPTOLENS_CURL_EMBED_CACERTS OFF CACHE BOOL "embed the ca certs in the library instead of using system default files?")

set (SRC "src/basic_Cryptolens.cpp" "src/ActivateError.cpp" "src/DataObject.cpp" "src/LicenseKey.cpp" "src/LicenseKeyChecker.cpp" "src/LicenseKeyInformation.cpp" "src/MachineCodeComputer_static.cpp" "third_party/base64_OpenBSD/base64.cpp")
set (LIBS "")

find_package(OpenSSL)
if (${OpenSSL_FOUND})
  set (SRC  ${SRC} "src/SignatureVerifier_OpenSSL.cpp")
  set (LIBS ${LIBS} crypto)
endif ()

find_package(CURL)
if (${CURL_FOUND})
  set (SRC ${SRC} "src/RequestHandler_curl.cpp")
  set (LIBS ${LIBS} curl ssl crypto)

  if ((${CRYPTOLENS_CURL_EMBED_CACERTS}) OR (${SKM_CURL_EMBED_CACERTS}))
    add_definitions (-DCRYPTOLENS_CURL_EMBED_CACERTS)
    set (SRC ${SRC} "src/RequestHandler_curl_cacerts.cpp")
  endif ()
endif ()

add_library (cryptolens STATIC ${SRC})
target_link_libraries (cryptolens ${LIBS})
target_include_directories (cryptolens PRIVATE "${cryptolens_SOURCE_DIR}/include/cryptolens")
target_include_directories (cryptolens PUBLIC "${cryptolens_SOURCE_DIR}/include")
set_property (TARGET cryptolens PROPERTY CXX_STANDARD 17)
set_property (TARGET cryptolens PROPERTY CXX_STANDARD_REQURED ON)

if (${CRYPTOLENS_BUILD_TESTS})
  add_subdirectory (tests)
endif ()
