cmake_minimum_required (VERSION 3.0.2)
project (skm)

set (SKM_BUILD_TESTS OFF CACHE BOOL "build tests?")
set (SKM_CURL_EMBED_CACERTS OFF CACHE BOOL "embed the ca certs in the library instead of using curl defaults?")

set (SRC  "src/ActivateError.cpp" "src/DataObject.cpp" "src/LicenseKey.cpp" "src/LicenseKeyChecker.cpp" "src/base64.cpp" "src/basic_SKM.cpp")
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

  if (${SKM_CURL_EMBED_CACERTS})
    add_definitions (-DSKM_CURL_EMBED_CACERTS)
    set (SRC ${SRC} "src/RequestHandler_curl_cacerts.cpp")
  endif ()
endif ()

add_library (skm STATIC ${SRC})
target_link_libraries(skm ${LIBS})
target_include_directories (skm PUBLIC "${skm_SOURCE_DIR}/include")
set_property(TARGET skm PROPERTY CXX_STANDARD 17)
set_property(TARGET skm PROPERTY CXX_STANDARD_REQURED ON)

if (${SKM_BUILD_TESTS})
  add_subdirectory (tests)
endif ()
