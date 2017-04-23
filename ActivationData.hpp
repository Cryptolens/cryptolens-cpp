#pragma once

namespace serialkeymanager_com {

// An immutable class representing an activated machine
// for a given serial key
class ActivationData {
private:
  std::string mid_;
  std::string ip_;
  std::string time_;
public:
  ActivationData
    (std::string mid
    , std::string ip
    , std::string time
    )
  : mid_(std::move(mid))
  , ip_(std::move(ip))
  , time_(std::move(time))
  { }

  // Returns the machine id
  std::string const& get_mid() const { return mid_; }

  // Returns the IP when the machine was activated the first time
  std::string const& get_ip() const { return ip_; }

  // Returns the time the machine was activated the first time
  std::string const& get_time() const { return time_; }
};

} // namespace serialkeymanager_com
