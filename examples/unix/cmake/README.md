# Cmake

This example uses Curl and OpenSSL, and dynamically links against these libraries. Thus,
these libraries must be available on the system. The following installs the libraries
on some common platforms:

```
Debian/Ubuntu:
$ apt-get install libcurl4-openssl-dev libssl-dev
Fedora/Red Hat:
$ yum install libcurl-devel openssl-devel
```

Build instructions (assuming this folder is the current working directory):

```
$ cmake ..
$ make -j8
```
