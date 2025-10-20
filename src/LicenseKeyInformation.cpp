#include "api.hpp"
#include "basic_Cryptolens.hpp"
#include "LicenseKeyInformation.hpp"
#include "ResponseParser_ArduinoJson7.hpp"

namespace cryptolens_io {

namespace v20190401 {

LicenseKeyInformation::LicenseKeyInformation()
{}

LicenseKeyInformation::LicenseKeyInformation(
  api::internal::main,
  int           product_id,
  std::uint64_t created,
  std::uint64_t expires,
  int           period,
  bool          block,
  bool          trial_activation,
  std::uint64_t sign_date,
  bool          f1,
  bool          f2,
  bool          f3,
  bool          f4,
  bool          f5,
  bool          f6,
  bool          f7,
  bool          f8,

  optional<int>                         id,
  optional<std::string>                 key,
  optional<std::string>                 notes,
  optional<int>                         global_id,
  optional<Customer>                    customer,
  optional<std::vector<ActivationData>> activated_machines,
  optional<int>                         maxnoofmachines,
  optional<std::string>                 allowed_machines,
  optional<std::vector<DataObject>>     data_objects
  )
  : product_id_(product_id)
  , created_(created)
  , expires_(expires)
  , period_(period)
  , block_(block)
  , trial_activation_(trial_activation)
  , sign_date_(sign_date)
  , f1_(f1)
  , f2_(f2)
  , f3_(f3)
  , f4_(f4)
  , f5_(f5)
  , f6_(f6)
  , f7_(f7)
  , f8_(f8)

  , id_(std::move(id))
  , key_(std::move(key))
  , notes_(std::move(notes))
  , global_id_(std::move(global_id))
  , customer_(std::move(customer))
  , activated_machines_(std::move(activated_machines))
  , maxnoofmachines_(std::move(maxnoofmachines))
  , allowed_machines_(std::move(allowed_machines))
  , data_objects_(std::move(data_objects))
  { };

#if 1
/**
 * Attempt to construct a LicenseKeyInformation from a RawLicenseKey
 */
optional<LicenseKeyInformation>
LicenseKeyInformation::make(basic_Error & e, RawLicenseKey const& raw_license_key)
{
  if (e) { return nullopt; }

  return LicenseKeyInformation::make_unsafe(e, raw_license_key.get_license());
}

/**
 * Attempt to construct a LicenseKeyInformation from an optional containing a RawLicenseKey
 */
optional<LicenseKeyInformation>
LicenseKeyInformation::make(basic_Error & e, optional<RawLicenseKey> const& raw_license_key)
{
  if (e) { return nullopt; }

  if (!raw_license_key) { return nullopt; }

  return LicenseKeyInformation::make(e, *raw_license_key);
}

/**
 * Attempt to construct a LicenseKeyInformation from a json string
 *
 * This is unsafe in the sense that by passing in the json string
 * directly there is no check of the cryptographic signature. The
 * signature verification is instead performed during construction of
 * a RawLicenseKey object. A LicenseKeyInformation object can then be constructed
 * from the RawLicenseKey using another static factory.
 */
optional<LicenseKeyInformation>
LicenseKeyInformation::make_unsafe(basic_Error & e, std::string const& license_key)
{
  if (e) { return nullopt; }

  ResponseParser_ArduinoJson7 response_parser(e);

  return response_parser.make_license_key_information_unsafe(e, license_key);
}
#endif

/**
 * Return a LicenseKeyChecker working on this LicenseKeyInformation object
 */
LicenseKeyChecker
LicenseKeyInformation::check() const {
  return LicenseKeyChecker(this);
}

/**
 * Returns the product Id of he license key
 */
int LicenseKeyInformation::get_product_id() const
{
  return product_id_;
}

/**
 * Returns the date and time the license key was created
 */
std::uint64_t
LicenseKeyInformation::get_created() const
{
  return created_;
}

/**
 * Returns the date and time the license key expires
 */
std::uint64_t
LicenseKeyInformation::get_expires() const
{
  return expires_;
}

/**
 * Returns the duration of current license cycle eg. 30 days
 */
int
LicenseKeyInformation::get_period() const
{
  return period_;
}

/**
 * Returns if the license key is blocked
 */
bool
LicenseKeyInformation::get_block() const
{
  return block_;
}

/**
 * Returns if trial activation is enabled
 */
bool
LicenseKeyInformation::get_trial_activation() const
{
  return trial_activation_;
}

/**
 * Returns the date the license key was created by the Web API
 */
std::uint64_t
LicenseKeyInformation::get_sign_date() const
{
  return sign_date_;
}

/**
 * Returns if the license key has feature 1
 */
bool
LicenseKeyInformation::get_f1() const
{
  return f1_;
}

/**
 * Returns if the license key has feature 2
 */
bool
LicenseKeyInformation::get_f2() const
{
  return f2_;
}

/**
 * Returns if the license key has feature 3
 */
bool
LicenseKeyInformation::get_f3() const
{
  return f3_;
}

/**
 * Returns if the license key has feature 4
 */
bool
LicenseKeyInformation::get_f4() const
{
  return f4_;
}

/**
 * Returns if the license key has feature 5
 */
bool
LicenseKeyInformation::get_f5() const
{
  return f5_;
}

/**
 * Returns if the license key has feature 6
 */
bool
LicenseKeyInformation::get_f6() const
{
  return f6_;
}

/**
 * Returns if the license key has feature 7
 */
bool
LicenseKeyInformation::get_f7() const
{
  return f7_;
}

/**
 * Returns if the license key has feature 8
 */
bool
LicenseKeyInformation::get_f8() const
{
  return f8_;
}

/**
 * Returns the Id of the license key
 */
optional<int> const&
LicenseKeyInformation::get_id() const
{
  return id_;
}

/**
 * Return the license key string, eg. ABCDE-EFGHI-JKLMO-PQRST
 */
optional<std::string> const&
LicenseKeyInformation::get_key() const
{
  return key_;
}

/**
 * Returns the notes field of the license key
 */
optional<std::string> const&
LicenseKeyInformation::get_notes() const
{
  return notes_;
}

/**
 * Returns a unique global identifier for the license key
 */
optional<int> const&
LicenseKeyInformation::get_global_id() const
{
  return global_id_;
}

/**
 * Returns the customer object assigned to the license key
 */
optional<Customer> const&
LicenseKeyInformation::get_customer() const
{
  return customer_;
}

/**
 * Returns the list of activated machines
 */
optional<std::vector<ActivationData>> const&
LicenseKeyInformation::get_activated_machines() const
{
  return activated_machines_;
}

/**
 * Returns the maximum number of machines/devices that may activate this
 * license key.
 */
optional<int> const&
LicenseKeyInformation::get_maxnoofmachines() const
{
  return maxnoofmachines_;
}

/**
 * Returns the machine codes of those devices that will be prioritized
 * during activation. Even if the limit is achieved, these will still be
 * activated.
 */
optional<std::string> const&
LicenseKeyInformation::get_allowed_machines() const
{
  return allowed_machines_;
}

/**
 * Returns the data objects associated with the license key.
 */
optional<std::vector<DataObject>> const&
LicenseKeyInformation::get_data_objects() const
{
  return data_objects_;
}

} // namespace v20190401

} // namespace cryptolens_io
