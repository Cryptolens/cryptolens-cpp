#pragma once

#include <string>

#include "optional.hpp"

namespace cryptolens_io {

namespace v20180502 {

// Internal functions used by the library for dealing with messages
// encoded with base64.

int
b64_pton(char const *src, unsigned char *target, size_t targsize);

optional<std::string>
b64_decode(std::string const& b64);

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
