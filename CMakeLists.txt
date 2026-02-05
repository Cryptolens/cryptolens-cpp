cmake_minimum_required (VERSION 3.5.0)
project (cryptolens)

set (CRYPTOLENS_BUILD_TESTS OFF CACHE BOOL "build tests?")
set (CRYPTOLENS_CURL_EMBED_CACERTS OFF CACHE BOOL "embed the ca certs in the library instead of using system default files?")
set(CRYPTOLENS_LIBRARY_TYPE "STATIC" CACHE STRING "Type of library to be created. Must be STATIC, SHARED or MODULE.")

set (SRC "src/ActivateError.cpp" "src/DataObject.cpp" "src/LicenseKey.cpp" "src/LicenseKeyChecker.cpp" "src/LicenseKeyInformation.cpp" "src/MachineCodeComputer_static.cpp" "src/RawLicenseKey.cpp" "src/ResponseParser_ArduinoJson7.cpp" "src/basic_SKM.cpp" "src/cryptolens_internals.cpp" "third_party/base64_OpenBSD/base64.cpp")

if(NOT WIN32)
  set (LIBS "pthread" "dl")

  find_package(OpenSSL)
  if (${OpenSSL_FOUND})
    if (${OPENSSL_VERSION} VERSION_LESS "3.0.0")
      set (SRC  ${SRC} "src/SignatureVerifier_OpenSSL.cpp")
    else ()
      set (SRC  ${SRC} "src/SignatureVerifier_OpenSSL3.cpp")
    endif ()

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

  set (CRYPTOLENS_BUILD_MACHINE_CODE_SYSTEMDDBUSINODES OFF CACHE BOOL "build with MachineCodeComputer_SystemdDBusInodes_SHA256?")
  if (CRYPTOLENS_BUILD_MACHINE_CODE_SYSTEMDDBUSINODES)
    list (APPEND SRC "src/MachineCodeComputer_SystemdDBusInodes_SHA256.cpp")
  endif()
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
target_include_directories (cryptolens PRIVATE "${cryptolens_SOURCE_DIR}/third_party/ArduinoJson7")
target_include_directories (cryptolens PUBLIC
    $<BUILD_INTERFACE:${cryptolens_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (${CRYPTOLENS_BUILD_TESTS})
  add_subdirectory (tests)
endif ()

# Installation
include(GNUInstallDirs)

install(TARGETS cryptolens
    EXPORT cryptolensTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${cryptolens_SOURCE_DIR}/include/cryptolens
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT cryptolensTargets
    FILE cryptolensTargets.cmake
    NAMESPACE cryptolens::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cryptolens
)

# Generate and install package config files
include(CMakePackageConfigHelpers)

# Create a basic config file
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfig.cmake.in
"@PACKAGE_INIT@

include(\"\${CMAKE_CURRENT_LIST_DIR}/cryptolensTargets.cmake\")

check_required_components(cryptolens)
")

configure_package_config_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cryptolens
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfigVersion.cmake
    VERSION 1.3.0
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cryptolensConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cryptolens
)
