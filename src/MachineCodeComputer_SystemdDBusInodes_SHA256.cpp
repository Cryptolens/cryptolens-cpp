#include <algorithm>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <sstream>

#include <sys/stat.h>

#include <openssl/sha.h>


#include <iostream>

#include "MachineCodeComputer_SystemdDBusInodes_SHA256.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace {

void
append_file(std::stringstream & s, char const* file)
{
  std::ifstream f(file, std::ios_base::in | std::ios_base::binary);
  if (f) {
    f >> std::noskipws;

    std::istream_iterator<char> b(f);
    std::istream_iterator<char> e;
    std::ostream_iterator<char> o(s);

    std::copy(b, e, o);
  }
}

void
append_inode(std::stringstream & s, char const* file)
{
  struct stat statbuf;
  int r;

  r = stat(file, &statbuf);
  if (r) { return; }

  s << statbuf.st_ino;
}

}  // anonymous namespace

std::string
MachineCodeComputer_SystemdDBusInodes_SHA256::get_machine_code(basic_Error & e)
{
  char HEX[] = "0123456789abcdef";
  std::stringstream s;

  append_file(s, "/etc/machine-id");
  s << "---";
  append_file(s, "/var/lib/dbus/machine-id");
  s << "---";

  append_inode(s, "/bin");
  s << "---";
  append_inode(s, "/etc");
  s << "---";
  append_inode(s, "/lib");
  s << "---";
  append_inode(s, "/root");
  s << "---";
  append_inode(s, "/sbin");
  s << "---";
  append_inode(s, "/usr");
  s << "---";
  append_inode(s, "/var");

  std::string unhashed = s.str();

  unsigned char digest[SHA256_DIGEST_LENGTH];

  SHA256((unsigned char *)unhashed.c_str(), unhashed.size(), digest);

  std::string machine_code;

  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    unsigned char x = digest[i];
    unsigned int x1 = (x & 0xF0) >> 4;
    unsigned int x2 = (x & 0x0F) >> 0;

    machine_code += HEX[x1];
    machine_code += HEX[x2];
  }

  return machine_code;
}

} // namespace v20190401

} // namespace cryptolens_io
