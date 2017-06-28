cmake_minimum_required (VERSION 2.6)

project (skm)

include_directories(${skm_SOURCE_DIR})

add_subdirectory (googletest/googletest)

add_library(skm STATIC ActivateError.cpp DataObject.cpp LicenseKey.cpp LicenseKeyChecker.cpp RequestHandler_curl.cpp RequestHandler_static.cpp SignatureVerifier_OpenSSL.cpp base64.cpp)

add_executable(example1 example1.cpp)
target_link_libraries(example1 skm curl crypto ssl)

add_executable(example2 example1.cpp)
target_link_libraries(example2 skm curl crypto ssl)

add_executable(tests tests.cpp)
target_link_libraries(tests skm gtest curl crypto ssl)
