g++ \
  -std=c++11 \
  -I include/cryptolens/ \
  -r \
  -o cryptolens.o \
  src/ActivateError.cpp \
  src/basic_SKM.cpp \
  src/cryptolens_internals.cpp \
  src/DataObject.cpp \
  src/LicenseKeyChecker.cpp \
  src/LicenseKey.cpp \
  src/LicenseKeyInformation.cpp \
  src/MachineCodeComputer_static.cpp \
  src/RawLicenseKey.cpp \
  src/RequestHandler_curl.cpp \
  src/ResponseParser_ArduinoJson5.cpp \
  src/SignatureVerifier_OpenSSL.cpp \
  third_party/base64_OpenBSD/base64.cpp


g++ \
  -std=c++11 \
  -I include/ \
  -o example_activate.out \
  examples/unix/example_activate.cpp \
  cryptolens.o \
  -lcurl \
  -lssl \
  -lcrypto
