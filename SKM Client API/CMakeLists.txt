cmake_minimum_required (VERSION 2.6)
project (skm)

set (BUILD_TESTS OFF CACHE BOOL "build tests?")
set (USE_OPENSSL OFF CACHE BOOL "is OpenSSL or equivalent available?")
set (USE_CURL    OFF CACHE BOOL "is libcurl available?")

add_library (skm STATIC ActivateError.cpp DataObject.cpp LicenseKey.cpp LicenseKeyChecker.cpp RequestHandler_curl.cpp SignatureVerifier_OpenSSL.cpp base64.cpp basic_SKM.cpp)
target_include_directories (skm PUBLIC ${skm_SOURCE_DIR})
set_property(TARGET skm PROPERTY CXX_STANDARD 11)

if (${BUILD_TESTS})
  add_subdirectory (tests)
endif ()
