cmake_minimum_required (VERSION 2.6)
project (skm)

set (BUILD_TESTS OFF CACHE BOOL "build tests?")
set (USE_OPENSSL OFF CACHE BOOL "is OpenSSL or equivalent available?")
set (USE_CURL    OFF CACHE BOOL "is libcurl available?")

#include_directories (${skm_SOURCE_DIR})
#include_directories (.)

add_library (skm STATIC ActivateError.cpp DataObject.cpp LicenseKey.cpp LicenseKeyChecker.cpp RequestHandler_curl.cpp RequestHandler_static.cpp SignatureVerifier_OpenSSL.cpp base64.cpp basic_SKM.cpp)

#add_executable(example1 examples/example1.cpp)
#target_link_libraries(example1 skm curl crypto ssl)

#add_executable(example2 examples/example2.cpp)
#target_link_libraries(example2 skm curl crypto ssl)

if (${BUILD_TESTS})
  add_subdirectory (tests)
endif ()
