FROM debian

RUN apt-get update \
 && apt-get install -y build-essential curl git vim cmake \
 && adduser --gecos "" --disabled-password builder

USER builder:builder
WORKDIR /home/builder
RUN CURL_URL="https://curl.haxx.se/download/curl-7.63.0.tar.xz" \
 && OPENSSL_URL="https://www.openssl.org/source/openssl-1.0.2q.tar.gz" \
 && curl -LO "$CURL_URL" \
 && curl -LO "$OPENSSL_URL"
RUN mkdir curl && tar xf curl*.tar* -C curl && rm curl*.tar*
RUN mkdir ssl  && tar xf *ssl*.tar* -C ssl  && rm *ssl*.tar*
RUN cd /home/builder/curl && mv curl* src && mkdir install
RUN cd /home/builder/ssl  && mv *ssl* src && mkdir install
RUN cd /home/builder/ssl/src \
 &&   ./config --prefix="/home/builder/ssl/install" -fPIC \
 &&   make -j8  \
 &&   make install
RUN cd /home/builder/curl/src \
 &&   ./configure --prefix="/home/builder/curl/install" -with-ssl="/home/builder/ssl/install" \
 &&   make -j8  \
 &&   make install

RUN git clone https://github.com/cryptolens/cryptolens-cpp.git

RUN mkdir -p /home/builder/cryptolens-cpp/examples/unix/static_docker/cmake/build \
 &&   cd     /home/builder/cryptolens-cpp/examples/unix/static_docker/cmake/build \
 &&   cmake .. \
 &&   make -j8

RUN mkdir -p /home/builder/cryptolens-cpp/examples/unix/static_docker/cc \
 &&   cd     /home/builder/cryptolens-cpp/examples/unix/static_docker/cc \
 &&   export CURL_ROOT="/home/builder/curl/install" \
 &&   export SSL_ROOT="/home/builder/ssl/install" \
 &&   export CRYPTOLENS_ROOT="/home/builder/cryptolens-cpp" \
 &&   g++ -c \
          -I "$CRYPTOLENS_ROOT/include/cryptolens" \
          "$CRYPTOLENS_ROOT/src/ActivateError.cpp" \
          "$CRYPTOLENS_ROOT/src/DataObject.cpp" \
          "$CRYPTOLENS_ROOT/src/LicenseKey.cpp" \
          "$CRYPTOLENS_ROOT/src/LicenseKeyChecker.cpp" \
          "$CRYPTOLENS_ROOT/src/LicenseKeyInformation.cpp" \
          "$CRYPTOLENS_ROOT/src/base64.cpp" \
          "$CRYPTOLENS_ROOT/src/basic_Cryptolens.cpp" \
 &&   g++ -c \
          -I "$SSL_ROOT/include" \
          -I "$CURL_ROOT/include" \
          -I "$CRYPTOLENS_ROOT/include/cryptolens" \
          "$CRYPTOLENS_ROOT/src/SignatureVerifier_OpenSSL.cpp" \
 &&   g++ -c \
          -I "$SSL_ROOT/include" \
          -I "$CURL_ROOT/include" \
          -I "$CRYPTOLENS_ROOT/include/cryptolens" \
          "$CRYPTOLENS_ROOT/src/RequestHandler_curl.cpp" \
 &&   g++ -c \
          -I "$SSL_ROOT/include" \
          -I "$CURL_ROOT/include" \
          -I "$CRYPTOLENS_ROOT/include" \
          "$CRYPTOLENS_ROOT/examples/unix/example_activate.cpp" \
 &&   g++ -pthread \
          -ldl \
          -o example_activate \
          *.o \
          "$CURL_ROOT/lib/libcurl.a" \
          "$SSL_ROOT/lib/libssl.a" \
          "$SSL_ROOT/lib/libcrypto.a"
