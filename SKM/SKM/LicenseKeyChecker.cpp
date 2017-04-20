#include "LicenseKeyChecker.hpp"

namespace serialkeymanager_com {

LicenseKeyChecker::LicenseKeyChecker(LicenseKey const* license_key)
: status_(true), key_(license_key)
{ }

LicenseKeyChecker::operator bool() const {
  return status_;
}

LicenseKeyChecker&
LicenseKeyChecker::has_feature(int feature) {
  switch (feature) {
  case 1:
    if (!key_->get_f1()) { status_ = false; }
  break;

  case 2:
    if (!key_->get_f2()) { status_ = false; }
  break;

  case 3:
    if (!key_->get_f3()) { status_ = false; }
  break;

  case 4:
    if (!key_->get_f4()) { status_ = false; }
  break;

  case 5:
    if (!key_->get_f5()) { status_ = false; }
  break;

  case 6:
    if (!key_->get_f6()) { status_ = false; }
  break;

  case 7:
    if (!key_->get_f7()) { status_ = false; }
  break;

  case 8:
    if (!key_->get_f8()) { status_ = false; }
  break;

  default:
    // TODO: Decide on how to handle invalid feature
  break;
  }

  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::has_not_feature(int feature) {
  switch (feature) {
  case 1:
    if (key_->get_f1()) { status_ = false; }
  break;

  case 2:
    if (key_->get_f2()) { status_ = false; }
  break;

  case 3:
    if (key_->get_f3()) { status_ = false; }
  break;

  case 4:
    if (key_->get_f4()) { status_ = false; }
  break;

  case 5:
    if (key_->get_f5()) { status_ = false; }
  break;

  case 6:
    if (key_->get_f6()) { status_ = false; }
  break;

  case 7:
    if (key_->get_f7()) { status_ = false; }
  break;

  case 8:
    if (key_->get_f8()) { status_ = false; }
  break;

  default:
    // TODO: Decide on how to handle invalid feature
  break;
  }

  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::has_expired(/* TODO: How to deal with dates in C++? */)
{
  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::has_not_expired(/* TODO: How to deal with dates in C++? */)
{
  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::is_blocked()
{
  if (!key_->get_block()) { status_ = false; }

  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::is_not_blocked()
{
  if (key_->get_block()) { status_ = false; }

  return *this;
}

LicenseKeyChecker&
LicenseKeyChecker::is_on_right_machine(std::string const& machine_code)
{
  // TODO: Fix is_on_right_machine
  //if (key_->get_machine_code() != machine_code) { status_ = false; }

  return *this;
}

} // namespace serialkeymanager_com
