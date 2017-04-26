#include "json.hpp"

#include "basic_SKM.hpp"
#include "LicenseKey.hpp"

namespace serialkeymanager_com {

using json = nlohmann::json;
using json_exception = nlohmann::detail::exception;

optional<LicenseKey>
LicenseKey::make(std::string const& license_key)
{
  json j;

  try {
    j = json::parse(license_key);
  } catch (json_exception & e) {
    return nullopt;
  }

  bool mandatory_missing =
      !( j["ProductId"].is_number_integer()
      && j["Created"].is_number_unsigned()
      && j["Expires"].is_number_unsigned()
      && j["Period"].is_number_integer()
      && j["Block"].is_boolean()
      && j["TrialActivation"].is_boolean()
      && j["SignDate"].is_number_unsigned()
      && j["F1"].is_boolean()
      && j["F2"].is_boolean()
      && j["F3"].is_boolean()
      && j["F4"].is_boolean()
      && j["F5"].is_boolean()
      && j["F6"].is_boolean()
      && j["F7"].is_boolean()
      && j["F8"].is_boolean()
      );

  if (mandatory_missing) { return nullopt; }

  LicenseKey key;

  key.product_id       = j["ProductId"];
  key.created          = j["Created"];
  key.expires          = j["Expires"];
  key.period           = j["Period"];
  key.block            = j["Block"];
  key.trial_activation = j["TrialActivation"];
  key.sign_date        = j["SignDate"];
  key.f1               = j["F1"];
  key.f2               = j["F2"];
  key.f3               = j["F3"];
  key.f4               = j["F4"];
  key.f5               = j["F5"];
  key.f6               = j["F6"];
  key.f7               = j["F7"];
  key.f8               = j["F8"];

  if(j["ID"].is_number_integer()) {
   int x = j["ID"];
   key.id = x;
  }

  if(j["Key"].is_string()) {
    std::string x = j["Key"];
    key.key = std::move(x);
  }

  if(j["Notes"].is_string()) {
    std::string x = j["Notes"];
    key.notes = std::move(x);
  }


  if(j["GlobalId"].is_number_integer()) {
    int x = j["GlobalId"];
    key.global_id = x;
  }

  if(j["Customer"].is_object()) {
    bool valid =
         j["Customer"]["Id"].is_number_integer()
      && j["Customer"]["Name"].is_string()
      && j["Customer"]["Email"].is_string()
      && j["Customer"]["CompanyName"].is_string()
      && j["Customer"]["Created"].is_number_unsigned();

    if (valid) {
      Customer x( j["Customer"]["Id"]
                , j["Customer"]["Name"]
                , j["Customer"]["Email"]
                , j["Customer"]["CompanyName"]
                , j["Customer"]["Created"]
                );
      key.customer = x;
    }
  }

  if(j["ActivatedMachines"].is_array()) {
    bool valid = true;
    std::vector<ActivationData> x;
    for (auto a : j["ActivatedMachines"]) {
      if (a["Mid"].is_string() && a["IP"].is_string() && a["Time"].is_number_unsigned()) {
        //x.push_back(ActivationData(a["Mid"], a["IP"], a["Time"]));
        x.emplace_back(a["Mid"], a["IP"], a["Time"]);
      } else {
        valid = false;
        break;
      }
    }

    if (valid) {
      key.activated_machines = std::move(x);
    }
  }

  if(j["MaxNoOfMachines"].is_number_integer()) {
    int x = j["MaxNoOfMachines"];
    key.maxnoofmachines = x;
  }

  if(j["AllowedMachines"].is_string()) {
    std::string x = j["AllowedMachines"];
    key.allowed_machines = std::move(x);
  }

  if(j["DataObjects"].is_array()) {
    bool valid = true;
    std::vector<DataObject> x;
    for (auto o : j["DataObjects"]) {
      if (  o["Id"].is_number_integer()
         && o["Name"].is_string()
         && o["StringValue"].is_string()
         && o["IntValue"].is_number_integer()
         )
      {
        x.emplace_back(o["Id"], o["Name"], o["StringValue"], o["IntValue"]);
      } else {
        valid = false;
        break;
      }
    }

    if (valid) {
      key.data_objects = std::move(x);
    }
  }

  return make_optional(key);
}

LicenseKeyChecker
LicenseKey::check() const {
  return LicenseKeyChecker(this);
}

int LicenseKey::get_product_id() const
{
  return product_id;
}

std::uint64_t
LicenseKey::get_created() const
{
  return created;
}

std::uint64_t
LicenseKey::get_expires() const
{
  return expires;
}

int
LicenseKey::get_period() const
{
  return period;
}

bool
LicenseKey::get_block() const
{
  return block;
}

bool
LicenseKey::get_trial_activation() const
{
  return trial_activation;
}

std::uint64_t
LicenseKey::get_sign_date() const
{
  return sign_date;
}

bool
LicenseKey::get_f1() const
{
  return f1;
}

bool
LicenseKey::get_f2() const
{
  return f2;
}

bool
LicenseKey::get_f3() const
{
  return f3;
}

bool
LicenseKey::get_f4() const
{
  return f4;
}

bool
LicenseKey::get_f5() const
{
  return f5;
}

bool
LicenseKey::get_f6() const
{
  return f6;
}

bool
LicenseKey::get_f7() const
{
  return f7;
}

bool
LicenseKey::get_f8() const
{
  return f8;
}

optional<int> const&
LicenseKey::get_id() const
{
  return id;
}

optional<std::string> const&
LicenseKey::get_key() const
{
  return key;
}

optional<std::string> const&
LicenseKey::get_notes() const
{
  return notes;
}

optional<int> const&
LicenseKey::get_global_id() const
{
  return global_id;
}

optional<Customer> const&
LicenseKey::get_customer() const
{
  return customer;
}

optional<std::vector<ActivationData>> const&
LicenseKey::get_activated_machines() const
{
  return activated_machines;
}

optional<int> const&
LicenseKey::get_maxnoofmachines() const
{
  return maxnoofmachines;
}

optional<std::string> const&
LicenseKey::get_allowed_machines() const
{
  return allowed_machines;
}

optional<std::vector<DataObject>> const&
LicenseKey::get_data_objects() const
{
  return data_objects;
}

} // namespace serialkeymanager_com
