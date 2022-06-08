#include "imports/ArduinoJson6/ArduinoJson.hpp"

#include "api.hpp"
#include "cryptolens_internals.hpp"
#include "LicenseKeyInformation.hpp"
#include "ResponseParser_ArduinoJson6.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace {

ArduinoJson::DeserializationError
deserialize(ArduinoJson::DynamicJsonDocument & j, std::string const& s)
{
  using namespace ArduinoJson;

  size_t n = j.capacity();
  if (n == 0) { n = 512; }

  while (true) {
    DeserializationError err = deserializeJson(j, s);

    if (err == DeserializationError::NoMemory) {
      n *= 2;
      j = DynamicJsonDocument(n);

      if (j.capacity() == 0) {
        return DeserializationError::NoMemory;
      }
    } else {
      return err;
    }
  }
}

} // namespace

optional<LicenseKeyInformation>
ResponseParser_ArduinoJson6::make_license_key_information(basic_Error & e, RawLicenseKey const& raw_license_key) const
{
  if (e) { return nullopt; }

  return ResponseParser_ArduinoJson6::make_license_key_information_unsafe(e, raw_license_key.get_license());
}

optional<LicenseKeyInformation>
ResponseParser_ArduinoJson6::make_license_key_information(basic_Error & e, optional<RawLicenseKey> const& raw_license_key) const
{
  if (e) { return nullopt; }

  if (!raw_license_key) { return nullopt; }

  return ResponseParser_ArduinoJson6::make_license_key_information(e, *raw_license_key);
}

optional<LicenseKeyInformation>
ResponseParser_ArduinoJson6::make_license_key_information_unsafe(basic_Error & e, std::string const& license_key) const
{
  if (e) { return nullopt; }

  using namespace ArduinoJson;
  DynamicJsonDocument j(1024);

  if (deserialize(j, license_key)) { e.set(api::main(), errors::Subsystem::Json, 3, 2); return nullopt; }

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

  if (mandatory_missing) { e.set(api::main(), errors::Subsystem::Json, 2, 5); return nullopt; }

  optional<int>                         id;
  optional<std::string>                 key;
  optional<std::string>                 notes;
  optional<int>                         global_id;
  optional<Customer>                    customer;
  optional<std::vector<ActivationData>> activated_machines;
  optional<int>                         maxnoofmachines;
  optional<std::string>                 allowed_machines;
  optional<std::vector<DataObject>>     data_objects;

  // TODO: Refactor all of these if-blocks to separate functions which takes the
  //       json object by reference and which immediately returns the optional
  if (j["ID"].is<unsigned long>()) {
    id = j["ID"].as<unsigned long>();
  }

  if (j["Key"].is<const char*>() && j["Key"].as<const char*>() != NULL) {
    std::string x(j["Key"].as<const char*>());
    key = std::move(x);
  }

  if (j["Notes"].is<const char*>() && j["Notes"].as<const char*>() != NULL) {
    std::string x(j["Notes"].as<const char*>());
    notes = std::move(x);
  }

  if (j["GlobalId"].is<unsigned long>()) {
    global_id = j["GlobalId"].as<unsigned long>();
  }

  if (j["Customer"].is<JsonObject>()) {
    JsonObject const c = j["Customer"].as<JsonObject>();

    bool valid =
         c["Id"].is<unsigned long>()
      && c["Created"].is<unsigned long>();

    if (valid) {
      customer = Customer(
          c["Id"].as<unsigned long>()
        , c["Name"].is<const char*>()        && c["Name"].as<const char*>() != NULL        ?  c["Name"].as<const char*>()        : ""
        , c["Email"].is<const char*>()       && c["Email"].as<const char*>() != NULL       ?  c["Email"].as<const char*>()       : ""
        , c["CompanyName"].is<const char*>() && c["CompanyName"].as<const char*>() != NULL ?  c["CompanyName"].as<const char*>() : ""
        , c["Created"].as<unsigned long>()
        );
    }
  }

  if (j["ActivatedMachines"].is<JsonArray>()) {
    bool valid = true;
    std::vector<ActivationData> v;
    JsonArray array = j["ActivatedMachines"].as<JsonArray>();
    for (auto const& x : array) {
      if (!x.is<JsonObject>()) {
        valid = false;
	break;
      }

      JsonObject const machine = x.as<JsonObject>();

      if (machine["Mid"].is<const char*>() && machine["Mid"].as<const char*>() != NULL &&
          machine["IP"].is<const char*>() && machine["IP"].as<const char*>() != NULL &&
          machine["Time"].is<unsigned long>()) {
        v.emplace_back(machine["Mid"], machine["IP"], machine["Time"]);
      } else {
        valid = false;
        break;
      }
    }

    if (valid) {
      activated_machines = std::move(v);
    }
  }

  if (j["MaxNoOfMachines"].is<unsigned long>()) {
    maxnoofmachines = j["MaxNoOfMachines"].as<unsigned long>();
  }

  if (j["AllowedMachines"].is<const char*>() && j["AllowedMachines"].as<const char*>() != NULL) {
    std::string x = j["AllowedMachines"].as<const char*>();
    allowed_machines = std::move(x);
  }

  if (j["DataObjects"].is<JsonArray>()) {
    bool valid = true;
    std::vector<DataObject> v;
    JsonArray const array = j["DataObjects"].as<JsonArray>();
    for (auto const& x : array) {
      if (!x.is<JsonObject>()) {
        valid = false;
        break;
      }

      JsonObject const dataobject = x.as<JsonObject>();

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
      data_objects = std::move(v);
    }
  }

  return make_optional(LicenseKeyInformation(
    api::internal::main(),
    j["ProductId"].as<unsigned long>(),
    j["Created"].as<unsigned long>(),
    j["Expires"].as<unsigned long>(),
    j["Period"].as<unsigned long>(),
    j["Block"].as<bool>(),
    j["TrialActivation"].as<bool>(),
    j["SignDate"].as<unsigned long>(),
    j["F1"].as<bool>(),
    j["F2"].as<bool>(),
    j["F3"].as<bool>(),
    j["F4"].as<bool>(),
    j["F5"].as<bool>(),
    j["F6"].as<bool>(),
    j["F7"].as<bool>(),
    j["F8"].as<bool>(),
    std::move(id),
    std::move(key),
    std::move(notes),
    std::move(global_id),
    std::move(customer),
    std::move(activated_machines),
    std::move(maxnoofmachines),
    std::move(allowed_machines),
    std::move(data_objects)
  ));
}

optional<std::pair<std::string, std::string>>
ResponseParser_ArduinoJson6::parse_activate_response(basic_Error & e, std::string const& server_response) const
{
  using namespace errors;
  api::main api;

  using namespace ArduinoJson;
  DynamicJsonDocument j(1024);

  if (deserialize(j, server_response)) { e.set(api, Subsystem::Json, 1, 2); return nullopt; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return nullopt;
    }

    int reason = internal::activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return nullopt;
  }

  if (!j["licenseKey"].is<char const*>() || j["licenseKey"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  if (!j["signature"].is<char const*>() || j["signature"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  return make_optional(std::make_pair(j["licenseKey"].as<char const*>(), j["signature"].as<char const*>()));
}


std::string
ResponseParser_ArduinoJson6::parse_create_trial_key_response(basic_Error & e, std::string const& server_response) const
{
  if (e) { return ""; }

  using namespace ::cryptolens_io::latest::errors;
  using namespace ::ArduinoJson;
  api::main api;
  DynamicJsonDocument j(1024);

  if (deserialize(j, server_response)) { e.set(api, Subsystem::Json, 7, 2); return ""; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return "";
    }

    int reason = internal::activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return "";
  }

  if (!j["key"].is<char const*>()) { e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY); return ""; }

  char const* key = j["key"].as<char const*>();

  return key ? key : "";
}

void
ResponseParser_ArduinoJson6::parse_deactivate_response(basic_Error & e, std::string const& server_response) const
{
  using namespace errors;
  api::main api;

  using namespace ArduinoJson;
  DynamicJsonDocument j(1024);

  if (deserialize(j, server_response)) { e.set(api, Subsystem::Json, 9, 4); return; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return;
    }

    int reason = internal::activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return;
  }
}

std::string
ResponseParser_ArduinoJson6::parse_last_message_response(basic_Error & e, std::string const& server_response) const
{
  if (e) { return ""; }

  using namespace ::cryptolens_io::latest::errors;
  using namespace ::ArduinoJson;
  api::main api;
  DynamicJsonDocument j(1024);

  if (deserialize(j, server_response)) { e.set(api, Subsystem::Json, 8, 9); return ""; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return "";
    }

    int reason = internal::activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return "";
  }

  if (!j["messages"].is<JsonArray>()) { return ""; }

  JsonArray const array = j["messages"].as<JsonArray>();
  int i_max = -1;
  int created_max = -1;
  for (size_t i = 0; i < array.size(); ++i) {
    if (!array[i].is<JsonObject>()) { continue; }

    JsonObject const msg = array[i].as<JsonObject>();
    if (msg["created"].is<int>() &&
        msg["content"].is<const char*>() && msg["content"].as<const char*>() != NULL)
    {
      int created = msg["created"].as<int>();
      if (created > created_max) { created_max = created; i_max = i; }
    }
  }

  if (i_max >= 0) { JsonObject const msg = array[i_max].as<JsonObject>(); return msg["content"]; }

  return "";
}

} // namespace v20190401

} // namespace cryptolens_io
