#pragma once

#include <optional>

namespace cryptolens_io {

namespace v20180502 {

template<typename T>
using optional = std::optional<T>;

using std::make_optional;
using std::nullopt;

} // namespace v20180502

} // namespace cryptolens_io
