#pragma once

#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class TautologyValidator_ {
public:
  TautologyValidator_(basic_Error & e) {}

  void validate(basic_Error & e, Env & env) {}
};

} // namespace v20190401

namespace latest {

template<typename Env>
using TautologyValidator_ = ::cryptolens_io::v20190401::TautologyValidator_<Env>;

} // namespace latest

} // namespace cryptolens_io
