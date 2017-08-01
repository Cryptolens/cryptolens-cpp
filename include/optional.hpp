#pragma once

#include <optional>

namespace serialkeymanager_com {

template<typename T>
using optional = std::optional<T>;

using std::make_optional;
using std::nullopt;

} // namespace serialkeymanager_com
