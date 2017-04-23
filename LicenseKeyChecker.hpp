#pragma once

#include "LicenseKey.hpp"

namespace serialkeymanager_com {

class LicenseKey;

class LicenseKeyChecker {
  bool status_;
  LicenseKey const* key_;

public:
  LicenseKeyChecker(LicenseKey const* license_key);

  explicit operator bool() const;

  LicenseKeyChecker& has_feature(int feature);
  LicenseKeyChecker& has_not_feature(int feature);
  LicenseKeyChecker& has_expired(/* TODO: How to deal with dates in C++? */);
  LicenseKeyChecker& has_not_expired(/* TODO: How to deal with dates in C++? */);
  LicenseKeyChecker& is_blocked();
  LicenseKeyChecker& is_not_blocked();
  LicenseKeyChecker& is_on_right_machine(std::string const& machine_code);
};

} // namespace serialkeymanager_com
