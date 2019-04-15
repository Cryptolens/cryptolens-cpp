#include "LicenseKeyChecker.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * Construct a LicenseKeyChecker from a LicenseKey object. This can also be
 * accomplished by calling the check() method on the LicenseKey object
 * instead. That is
 *
 *     license_key.check()
 *
 * instead of
 *
 *     LicenseKeyChecker(license_key)
 */
LicenseKeyChecker::LicenseKeyChecker(LicenseKeyInformation const* license_key)
: status_(true), key_(license_key)
{ }

/**
 * Explicit conversion to bool allowing the LicenseKeyChecker
 * object to be used in e.g. control flow statements:
 *
 *     if (license_key.check().has_feature(4)) {
 *       DO_SOMETHING
 *     }
 */
LicenseKeyChecker::operator bool() const {
  return status_;
}

/**
 * Check that the underlying LicenseKey object has a certain feature.
 */
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

/**
 * Check that the underlying LicenseKey object does not have a
 * certain feature.
 */
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

/**
 * Check that the underlying LicenseKey object has expired.
 *
 * Time is given as a unix time stamp measured in seconds.
 */
LicenseKeyChecker&
LicenseKeyChecker::has_expired(std::uint64_t now)
{
  if (now < key_->get_expires()) {
    status_ = false;
  }

  return *this;
}

/**
 * Check that the underlying LicenseKey object has not expired.
 *
 * Time is given as a unix time stamp measured in seconds.
 */
LicenseKeyChecker&
LicenseKeyChecker::has_not_expired(std::uint64_t now)
{
  if (key_->get_expires() < now) {
    status_ = false;
  }

  return *this;
}

/**
 * Check that the underlying LicenseKey object is blocked.
 */
LicenseKeyChecker&
LicenseKeyChecker::is_blocked()
{
  if (!key_->get_block()) { status_ = false; }

  return *this;
}

/**
 * Check that the underlying LicenseKey object is not blocked.
 */
LicenseKeyChecker&
LicenseKeyChecker::is_not_blocked()
{
  if (key_->get_block()) { status_ = false; }

  return *this;
}

/**
 * Check that machine_code is among the allowed machines for the
 * underlying LicenseKey object.
 */
LicenseKeyChecker&
LicenseKeyChecker::is_on_right_machine(std::string const& machine_code)
{
  if (key_->get_activated_machines()) {
    for (auto const& m : *(key_->get_activated_machines())) {
      if (m.get_mid() == machine_code) {
        return *this;
      }
    }
  }

  status_ = false;
  return *this;
}

} // namespace v20190401

} // namespace cryptolens_io
