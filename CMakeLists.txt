cmake_minimum_required (VERSION 3.0.2)
project (cryptolens)

set (CRYPTOLENS_BUILD_TESTS OFF CACHE BOOL "build tests?")
set (CRYPTOLENS_CURL_EMBED_CACERTS OFF CACHE BOOL "embed the ca certs in the library instead of using system default files?")
set(CRYPTOLENS_LIBRARY_TYPE "STATIC" CACHE STRING "Type of library to be created. Must be STATIC, SHARED or MODULE.")

set (SRC "src/ActivateError.cpp" "src/DataObject.cpp" "src/LicenseKey.cpp" "src/LicenseKeyChecker.cpp" "src/LicenseKeyInformation.cpp" "src/MachineCodeComputer_static.cpp" "src/RawLicenseKey.cpp" "src/ResponseParser_ArduinoJson5.cpp" "src/basic_SKM.cpp" "src/cryptolens_internals.cpp" "third_party/base64_OpenBSD/base64.cpp")

if(NOT WIN32)
  set (LIBS "pthread" "dl")

  find_package(OpenSSL)
  if (${OpenSSL_FOUND})
  set (SRC  ${SRC} "src/SignatureVerifier_OpenSSL3.cpp")
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
else()
    add_definitions (-DUNICODE -D_UNICODE)

    # Windows specific source files
    list (APPEND SRC "src/RequestHandler_WinHTTP.cpp" "src/SignatureVerifier_CryptoAPI.cpp")
    list (APPEND LIBS "winhttp")

    set (CRYPTOLENS_BUILD_MACHINE_CODE_COM OFF CACHE BOOL "build with MachineCodeComputer_COM?")
    if (CRYPTOLENS_BUILD_MACHINE_CODE_COM)
        list (APPEND SRC "src/MachineCodeComputer_COM.cpp" "third_party/curl/isunreserved.cpp")
        list (APPEND LIBS "Iphlpapi")
    endif()
endif()



add_library (cryptolens ${CRYPTOLENS_LIBRARY_TYPE} ${SRC})
target_link_libraries (cryptolens ${LIBS})
target_include_directories (cryptolens PRIVATE "${cryptolens_SOURCE_DIR}/include/cryptolens")
target_include_directories (cryptolens PUBLIC "${cryptolens_SOURCE_DIR}/include")
set_property (TARGET cryptolens PROPERTY CXX_STANDARD 11)
set_property (TARGET cryptolens PROPERTY CXX_STANDARD_REQUIRED ON)

if (${CRYPTOLENS_BUILD_TESTS})
  add_subdirectory (tests)
endif ()
