cmake_minimum_required (VERSION 2.6)
project (skm)

set (SKM_BUILD_TESTS OFF CACHE BOOL "build tests?")
set (SKM_USE_OPENSSL OFF CACHE BOOL "is OpenSSL or equivalent available?")
set (SKM_USE_CURL    OFF CACHE BOOL "is libcurl available?")

set (SRC  "src/ActivateError.cpp" "src/DataObject.cpp" "src/LicenseKey.cpp" "src/LicenseKeyChecker.cpp" "src/base64.cpp" "src/basic_SKM.cpp")
set (LIBS "")

if (${SKM_USE_OPENSSL})
  find_package(OpenSSL)
  set (SRC  ${SRC} "src/SignatureVerifier_OpenSSL.cpp")
  set (LIBS ${LIBS} crypto)
endif ()

if (${SKM_USE_CURL})
  find_package(CURL)
  set (SRC ${SRC} "src/RequestHandler_curl.cpp")
  set (LIBS ${LIBS} curl ssl crypto)
endif ()

message (${LIBS})

add_library (skm STATIC ${SRC})
target_link_libraries(skm ${LIBS})
target_include_directories (skm PUBLIC "${skm_SOURCE_DIR}/include")
set_property(TARGET skm PROPERTY CXX_STANDARD 11)

if (${SKM_BUILD_TESTS})
  add_subdirectory (tests)
endif ()
