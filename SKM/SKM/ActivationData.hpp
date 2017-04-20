#pragma once

namespace serialkeymanager_com {

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

  std::string const& get_mid() const { return mid_; }
  std::string const& get_ip() const { return ip_; }
  std::string const& get_time() const { return time_; }
};

} // namespace serialkeymanager_com
