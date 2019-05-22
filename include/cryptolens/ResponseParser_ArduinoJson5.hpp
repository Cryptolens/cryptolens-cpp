#pragma once

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

class ResponseParser_ArduinoJson5 {
public:
  ResponseParser_ArduinoJson5(basic_Error & e) {}
};

} // namespace latest

namespace latest {

using ResponseParser_ArduinoJson5 = ::cryptolens_io::v20190401::ResponseParser_ArduinoJson5;

} // namespace latest

} // namespace cryptolens_io
