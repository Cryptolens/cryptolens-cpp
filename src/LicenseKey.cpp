#include "ArduinoJson.hpp"

#include "api.hpp"
#include "basic_SKM.hpp"
#include "LicenseKey.hpp"

namespace serialkeymanager_com {

using namespace ArduinoJson;



/**
 * Attempt to construct a LicenseKey from a RawLicenseKey
 */
optional<LicenseKey>
LicenseKey::make(basic_Error & e, RawLicenseKey const& raw_license_key)
{
  if (e) { return nullopt; }

  return LicenseKey::make_unsafe(e, raw_license_key.get_license());
}

/**
 * Attempt to construct a LicenseKey from an optional containing a RawLicenseKey
 */
optional<LicenseKey>
LicenseKey::make(basic_Error & e, optional<RawLicenseKey> const& raw_license_key)
{
  if (e) { return nullopt; }

  if (!raw_license_key) { return nullopt; }

  return LicenseKey::make(e, *raw_license_key);
}

/**
 * Attempt to construct a LicenseKey from a json string
 *
 * This is unsafe in the sense that by passing in the json string
 * directly there is no check of the cryptographic signature. The
 * signature verification is instead performed during construction of
 * a RawLicenseKey object. A LicenseKey object can then be constructed
 * from the RawLicenseKey using the static factory.
 */
optional<LicenseKey>
LicenseKey::make_unsafe(basic_Error & e, std::string const& license_key)
{
  if (e) { return nullopt; }

  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(license_key);

  if (!j.success()) { e.set(api::main(), errors::Subsystem::Json); return nullopt; }

  bool mandatory_missing =
      !( j["ProductId"].is<unsigned long>()
      && j["Created"].is<unsigned long>()
      && j["Expires"].is<unsigned long>()
      && j["Period"].is<unsigned long>()
      && j["Block"].is<bool>()
      && j["TrialActivation"].is<bool>()
      && j["SignDate"].is<unsigned long>()
      && j["F1"].is<bool>()
      && j["F2"].is<bool>()
      && j["F3"].is<bool>()
      && j["F4"].is<bool>()
      && j["F5"].is<bool>()
      && j["F6"].is<bool>()
      && j["F7"].is<bool>()
      && j["F8"].is<bool>()
      );

  if (mandatory_missing) { e.set(api::main(), errors::Subsystem::Json); return nullopt; }

  LicenseKey key;

  key.product_id       = j["ProductId"].as<unsigned long>();
  key.created          = j["Created"].as<unsigned long>();
  key.expires          = j["Expires"].as<unsigned long>();
  key.period           = j["Period"].as<unsigned long>();
  key.block            = j["Block"].as<bool>();
  key.trial_activation = j["TrialActivation"].as<bool>();
  key.sign_date        = j["SignDate"].as<unsigned long>();
  key.f1               = j["F1"].as<bool>();
  key.f2               = j["F2"].as<bool>();
  key.f3               = j["F3"].as<bool>();
  key.f4               = j["F4"].as<bool>();
  key.f5               = j["F5"].as<bool>();
  key.f6               = j["F6"].as<bool>();
  key.f7               = j["F7"].as<bool>();
  key.f8               = j["F8"].as<bool>();

  if (j["ID"].is<unsigned long>()) {
    key.id = j["ID"].as<unsigned long>();
  }

  if (j["Key"].is<const char*>() && j["Key"].as<const char*>() != NULL) {
    std::string x(j["Key"].as<const char*>());
    key.key = std::move(x);
  }

  if (j["Notes"].is<const char*>() && j["Notes"].as<const char*>() != NULL) {
    std::string x(j["Notes"].as<const char*>());
    key.notes = std::move(x);
  }

  if (j["GlobalId"].is<unsigned long>()) {
    key.global_id = j["GlobalId"].as<unsigned long>();
  }

  if (j["Customer"].is<const JsonObject&>()) {
    JsonObject const& customer = j["Customer"].as<const JsonObject&>();

    bool valid =
         customer["Id"].is<unsigned long>()
      && customer["Name"].is<const char*>() && customer["Name"].as<const char*>() != NULL
      && customer["Email"].is<const char*>() && customer["Email"].as<const char*>() != NULL
      && customer["CompanyName"].is<const char*>() && customer["CompanyName"].as<const char*>() != NULL
      && customer["Created"].is<unsigned long>();

    if (valid) {
      Customer x( customer["Id"].as<unsigned long>()
                , customer["Name"].as<const char*>()
                , customer["Email"].as<const char*>()
                , customer["CompanyName"].as<const char*>()
                , customer["Created"].as<unsigned long>()
		);
      key.customer = std::move(x);
    }
  }

  if (j["ActivatedMachines"].is<const JsonArray&>()) {
    bool valid = true;
    std::vector<ActivationData> v;
    JsonArray const& array = j["ActivatedMachines"].as<const JsonArray&>();
    for (auto const& x : array) {
      if (!x.is<const JsonObject&>()) {
        valid = false;
	break;
      }

      JsonObject const& machine = x.as<const JsonObject&>();

      if (machine["Mid"].is<const char*>() && machine["Mid"].as<const char*>() != NULL &&
          machine["IP"].is<const char*>() && machine["IP"].as<const char*>() != NULL &&
          machine["Time"].is<unsigned long>()) {
        //x.push_back(ActivationData(machine["Mid"], machine["IP"], machine["Time"]));
        v.emplace_back(machine["Mid"], machine["IP"], machine["Time"]);
      } else {
        valid = false;
        break;
      }
    }

    if (valid) {
      key.activated_machines = std::move(v);
    }
  }

  if (j["MaxNoOfMachines"].is<unsigned long>()) {
    key.maxnoofmachines = j["MaxNoOfMachines"].as<unsigned long>();
  }

  if (j["AllowedMachines"].is<const char*>() && j["AllowedMachines"].as<const char*>() != NULL) {
    std::string x = j["AllowedMachines"].as<const char*>();
    key.allowed_machines = std::move(x);
  }

  if (j["DataObjects"].is<const JsonArray&>()) {
    bool valid = true;
    std::vector<DataObject> v;
    JsonArray const& array = j["DataObjects"].as<const JsonArray&>();
    for (auto const& x : array) {
      if (!x.is<const JsonObject&>()) {
        valid = false;
        break;
      }

      JsonObject const& dataobject = x.as<const JsonObject&>();

      if (  dataobject["Id"].is<unsigned long>()
         && dataobject["Name"].is<const char*>() && dataobject["Name"].as<const char*>() != NULL
         && dataobject["StringValue"].is<const char*>() && dataobject["StringValue"].as<const char*>() != NULL
         && dataobject["IntValue"].is<unsigned long>()
         )
      {
        v.emplace_back( dataobject["Id"].as<unsigned long>()
                      , dataobject["Name"].as<const char*>()
                      , dataobject["StringValue"].as<const char*>()
                      , dataobject["IntValue"].as<unsigned long>()
                      );
      } else {
        valid = false;
        break;
      }
    }

    if (valid) {
      key.data_objects = std::move(v);
    }
  }

  return make_optional(key);
}

/**
 * Return a LicenseKeyChecker working on this LicenseKey object
 */
LicenseKeyChecker
LicenseKey::check() const {
  return LicenseKeyChecker(this);
}

/**
 * Returns the product Id of he license key
 */
int LicenseKey::get_product_id() const
{
  return product_id;
}

/**
 * Returns the date and time the license key was created
 */
std::uint64_t
LicenseKey::get_created() const
{
  return created;
}

/**
 * Returns the date and time the license key expires
 */
std::uint64_t
LicenseKey::get_expires() const
{
  return expires;
}

/**
 * Returns the duration of current license cycle eg. 30 days
 */
int
LicenseKey::get_period() const
{
  return period;
}

/**
 * Returns if the license key is blocked
 */
bool
LicenseKey::get_block() const
{
  return block;
}

/**
 * Returns if trial activation is enabled
 */
bool
LicenseKey::get_trial_activation() const
{
  return trial_activation;
}

/**
 * Returns the date the license key was created by the Web API
 */
std::uint64_t
LicenseKey::get_sign_date() const
{
  return sign_date;
}

/**
 * Returns if the license key has feature 1
 */
bool
LicenseKey::get_f1() const
{
  return f1;
}

/**
 * Returns if the license key has feature 2
 */
bool
LicenseKey::get_f2() const
{
  return f2;
}

/**
 * Returns if the license key has feature 3
 */
bool
LicenseKey::get_f3() const
{
  return f3;
}

/**
 * Returns if the license key has feature 4
 */
bool
LicenseKey::get_f4() const
{
  return f4;
}

/**
 * Returns if the license key has feature 5
 */
bool
LicenseKey::get_f5() const
{
  return f5;
}

/**
 * Returns if the license key has feature 6
 */
bool
LicenseKey::get_f6() const
{
  return f6;
}

/**
 * Returns if the license key has feature 7
 */
bool
LicenseKey::get_f7() const
{
  return f7;
}

/**
 * Returns if the license key has feature 8
 */
bool
LicenseKey::get_f8() const
{
  return f8;
}

/**
 * Returns the Id of the license key
 */
optional<int> const&
LicenseKey::get_id() const
{
  return id;
}

/**
 * Return the license key string, eg. ABCDE-EFGHI-JKLMO-PQRST
 */
optional<std::string> const&
LicenseKey::get_key() const
{
  return key;
}

/**
 * Returns the notes field of the license key
 */
optional<std::string> const&
LicenseKey::get_notes() const
{
  return notes;
}

/**
 * Returns a unique global identifier for the license key
 */
optional<int> const&
LicenseKey::get_global_id() const
{
  return global_id;
}

/**
 * Returns the customer object assigned to the license key
 */
optional<Customer> const&
LicenseKey::get_customer() const
{
  return customer;
}

/**
 * Returns the list of activated machines
 */
optional<std::vector<ActivationData>> const&
LicenseKey::get_activated_machines() const
{
  return activated_machines;
}

/**
 * Returns the maximum number of machines/devices that may activate this
 * license key.
 */
optional<int> const&
LicenseKey::get_maxnoofmachines() const
{
  return maxnoofmachines;
}

/**
 * Returns the machine codes of those devices that will be prioritized
 * during activation. Even if the limit is achieved, these will still be
 * activated.
 */
optional<std::string> const&
LicenseKey::get_allowed_machines() const
{
  return allowed_machines;
}

/**
 * Returns the data objects associated with the license key.
 */
optional<std::vector<DataObject>> const&
LicenseKey::get_data_objects() const
{
  return data_objects;
}

} // namespace serialkeymanager_com
