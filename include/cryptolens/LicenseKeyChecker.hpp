#pragma once

#include "LicenseKeyInformation.hpp"

namespace cryptolens_io {

namespace v20180502 {

class LicenseKeyInformation;

class LicenseKeyChecker {
  bool status_;
  LicenseKeyInformation const* key_;

public:
  LicenseKeyChecker(LicenseKeyInformation const* license_key);

  explicit operator bool() const;

  LicenseKeyChecker& has_feature(int feature);
  LicenseKeyChecker& has_not_feature(int feature);
  LicenseKeyChecker& has_expired(std::uint64_t now);
  LicenseKeyChecker& has_not_expired(std::uint64_t now);
  LicenseKeyChecker& is_blocked();
  LicenseKeyChecker& is_not_blocked();
  LicenseKeyChecker& is_on_right_machine(std::string const& machine_code);
};

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
