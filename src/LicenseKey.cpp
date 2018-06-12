#include "api.hpp"
#include "basic_SKM.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyInformation.hpp"
#include "RawLicenseKey.hpp"

namespace serialkeymanager_com {

LicenseKey::LicenseKey(LicenseKeyInformation && license_key_information, RawLicenseKey && raw_license_key)
: info_(license_key_information), raw_(raw_license_key)
{ }

#if 0
LicenseKey
LicenseKey::make(basic_Error & e, LicenseKeyInformation && license_key_information, RawLicenseKey && raw_license_key)
{
  if (e) { return nullopt; }

  return LicenseKey(license_key_information, raw_license_key);
}

LicenseKey
LicenseKey::make(basic_Error & e, RawLicenseKey && raw_license_key)
{
  if (e) { return nullopt; }

  optional<LicenseKeyInformation> license_key_information = LicenseKeyInformation::make(e, raw_license_key);

  if (e) { return nullopt; }
  if (!license_key_information) { return nullopt; }
  // TODO: Set call location

  return LicenseKey(std::move(*license_key_information), raw_license_key);
}
#endif

/**
 * Return a LicenseKeyChecker working on this LicenseKey object
 */
LicenseKeyChecker
LicenseKey::check() const {
  return LicenseKeyChecker(&(this->info_));
}

/**
 * Returns the product Id of the license key
 */
int LicenseKey::get_product_id() const
{
  return info_.get_product_id();
}

/**
 * Returns the date and time the license key was created
 *
 * Format is a unix time stamp in seconds.
 */
std::uint64_t
LicenseKey::get_created() const
{
  return info_.get_created();
}

/**
 * Returns the date and time the license key expires
 *
 * Format is a unix time stamp in seconds.
 */
std::uint64_t
LicenseKey::get_expires() const
{
  return info_.get_expires();
}

/**
 * Returns the duration of current license cycle, eg. 30 days
 *
 * Format is given as number of days.
 */
int
LicenseKey::get_period() const
{
  return info_.get_period();
}

/**
 * Returns if the license key is blocked
 */
bool
LicenseKey::get_block() const
{
  return info_.get_block();
}

/**
 * Returns if trial activation is enabled
 */
bool
LicenseKey::get_trial_activation() const
{
  return info_.get_trial_activation();
}

/**
 * Returns the date the license key was created by the Web API
 *
 * Format is a unix time stamp in seconds.
 */
std::uint64_t
LicenseKey::get_sign_date() const
{
  return info_.get_sign_date();
}

/**
 * Returns if the license key has feature 1
 */
bool
LicenseKey::get_f1() const
{
  return info_.get_f1();
}

/**
 * Returns if the license key has feature 2
 */
bool
LicenseKey::get_f2() const
{
  return info_.get_f2();
}

/**
 * Returns if the license key has feature 3
 */
bool
LicenseKey::get_f3() const
{
  return info_.get_f3();
}

/**
 * Returns if the license key has feature 4
 */
bool
LicenseKey::get_f4() const
{
  return info_.get_f4();
}

/**
 * Returns if the license key has feature 5
 */
bool
LicenseKey::get_f5() const
{
  return info_.get_f5();
}

/**
 * Returns if the license key has feature 6
 */
bool
LicenseKey::get_f6() const
{
  return info_.get_f6();
}

/**
 * Returns if the license key has feature 7
 */
bool
LicenseKey::get_f7() const
{
  return info_.get_f7();
}

/**
 * Returns if the license key has feature 8
 */
bool
LicenseKey::get_f8() const
{
  return info_.get_f8();
}

/**
 * Returns the Id of the license key
 */
optional<int> const&
LicenseKey::get_id() const
{
  return info_.get_id();
}

/**
 * Return the license key string, eg. ABCDE-EFGHI-JKLMO-PQRST
 */
optional<std::string> const&
LicenseKey::get_key() const
{
  return info_.get_key();
}

/**
 * Returns the notes field of the license key
 */
optional<std::string> const&
LicenseKey::get_notes() const
{
  return info_.get_notes();
}

/**
 * Returns a unique global identifier for the license key
 */
optional<int> const&
LicenseKey::get_global_id() const
{
  return info_.get_global_id();
}

/**
 * Returns the customer object assigned to the license key
 */
optional<Customer> const&
LicenseKey::get_customer() const
{
  return info_.get_customer();
}

/**
 * Returns the list of activated machines
 */
optional<std::vector<ActivationData>> const&
LicenseKey::get_activated_machines() const
{
  return info_.get_activated_machines();
}

/**
 * Returns the maximum number of machines/devices that may activate this
 * license key.
 */
optional<int> const&
LicenseKey::get_maxnoofmachines() const
{
  return info_.get_maxnoofmachines();
}

/**
 * Returns the machine codes of those devices that will be prioritized
 * during activation. Even if the limit is achieved, these will still be
 * activated.
 */
optional<std::string> const&
LicenseKey::get_allowed_machines() const
{
  return info_.get_allowed_machines();
}

/**
 * Returns the data objects associated with the license key.
 */
optional<std::vector<DataObject>> const&
LicenseKey::get_data_objects() const
{
  return info_.get_data_objects();
}

} // namespace serialkeymanager_com
