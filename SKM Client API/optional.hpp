#pragma once

#include <experimental/optional>

namespace serialkeymanager_com {

template<typename T>
using optional = std::experimental::optional<T>;

using std::experimental::make_optional;
using std::experimental::nullopt;

} // namespace serialkeymanager_com
