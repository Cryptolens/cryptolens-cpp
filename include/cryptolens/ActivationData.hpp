#pragma once

#include <cstdint>
#include <string>

#include "imports/std/optional"

namespace cryptolens_io {

namespace v20190401 {

// An immutable class representing an activated machine
// for a given serial key
class ActivationData {
private:
  std::string mid_;
  std::string ip_;
  std::uint64_t time_;
  optional<std::string> friendly_name_;
public:
  ActivationData
    ( std::string mid
    , std::string ip
    , std::uint64_t time
    )
  : mid_(std::move(mid))
  , ip_(std::move(ip))
  , time_(time)
  , friendly_name_()
  { }

  ActivationData
    ( std::string mid
    , std::string ip
    , std::uint64_t time
    , std::string friendly_name
    )
  : mid_(std::move(mid))
  , ip_(std::move(ip))
  , time_(time)
  , friendly_name_(friendly_name)
  { }

  // Returns the machine id
  std::string const& get_mid() const { return mid_; }

  // Returns the IP when the machine was activated the first time
  std::string const& get_ip() const { return ip_; }

  // Returns the time the machine was activated the first time
  std::uint64_t get_time() const { return time_; }

  // Returns an optional with the friendly name for the machine. If the machine does not have a friendly name, the optional is empty and otherwise contains a string with the friendly name
  optional<std::string> const& get_friendly_name() const { return friendly_name_; }
};

} // namespace v20190401

namespace v20180502 {

using ::cryptolens_io::v20190401::ActivationData;

} // namespace v20180502

namespace latest {

using ::cryptolens_io::v20190401::ActivationData;

} // namespace latest

} // namespace cryptolens_io
