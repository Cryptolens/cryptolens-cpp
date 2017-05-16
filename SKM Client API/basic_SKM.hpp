#pragma once

#include <string>
#include <unordered_map>

#include "optional.hpp"

#include "ArduinoJson.hpp"

#include "RawLicenseKey.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyChecker.hpp"

namespace serialkeymanager_com {

using namespace ArduinoJson;

// Helper class to work with the FieldsToReturn parameter when
// making a Activate request to the Web API.
//
// This class is not terribly important and the FieldsToReturn
// parameter should usually be set server side by setting
// a Feature Lock for the license key
struct FieldsToReturn {
  static const int ID = 1 << 1;
  static const int KEY = 1 << 2;
  static const int NOTES = 1 << 3;
  static const int GLOBALID = 1 << 4;
  static const int CUSTOMER = 1 << 5;
  static const int ACTIVATEDMACHINES = 1 << 6;
  static const int MAXNOOFMACHINES = 1 << 8;
  static const int ALLOWEDMACHINES = 1 << 7;
  static const int DATAOBJECTS = 1 << 9;

  static bool expect_id(int n) {
    return n == 0 || (n & ID);
  }

  static bool expect_key(int n) {
    return n == 0 || (n & KEY);
  }

  static bool expect_notes(int n) {
    return n == 0 || (n & NOTES);
  }

  static bool expect_global_id(int n) {
    return n == 0 || (n & GLOBALID);
  }

  static bool expect_customer(int n) {
    return n == 0 || (n & CUSTOMER);
  }

  static bool expect_activated_machines(int n) {
    return n == 0 || (n & ACTIVATEDMACHINES);
  }

  static bool expect_maxnoofmachines(int n) {
    return n == 0 || (n & MAXNOOFMACHINES);
  }

  static bool expect_allowed_machines(int n) {
    return n == 0 || (n & ALLOWEDMACHINES);
  }

  static bool expect_dataobjects(int n) {
    return n == 0 || (n & DATAOBJECTS);
  }
};

// Function for handling a response to an Activate request from
// the SKM Web API
template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(response);

  if (!j.success()) { return nullopt; }

  if (!j["licenseKey"].is<char const*>() || j["licenseKey"].as<char const*>() == NULL) {
    return nullopt;
  }

  if (!j["signature"].is<char const*>() || j["signature"].as<char const*>() == NULL) {
    return nullopt;
  }

  return RawLicenseKey::make(
             signature_verifier
           , j["licenseKey"].as<char const*>()
           , j["signature"].as<char const*>()
	   );
}

// Function for handling a response to an Deactivate request from
// the SKM Web API
template<typename SignatureVerifier>
bool
handle_deactivate
  ( SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(response);

  if (!j.success()) { return false; }

  if (!j["Result"].is<int>()) {
    return false;
  }

  return j["Result"].as<int>() == 0;
}

// This class makes it possible to interact with the SKM Web API. Among the
// various methods available in the Web API the only ones currently supported
// in the C++ API are Activate and Deactivate.
//
// This class uses two policy classes, SignatureVerifier and RequestHandler,
// which are responsible for handling verification of signatures and making
// requests to the Web API, respectivly.
template<typename RequestHandler, typename SignatureVerifier>
class basic_SKM
{
public:
  basic_SKM() { }

  // Make an Activate request to the SKM Web API
  //
  // Arguments:
  //   token - acces token to use
  //   product_id - the product id
  //   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
  //   machine_code - the machine code, i.e. a string that identifies a device
  //                  for activation.
  //
  // Returns:
  //   An optional with a RawLicenseKey representing if the request was
  //   successful or not.
  optional<RawLicenseKey>
  activate
    ( std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    )
  {
    std::unordered_map<std::string,std::string> args;
    args["token"] = token;
    args["ProductId"] = product_id;
    args["Key"] = key;
    args["Sign"] = "true";
    args["MachineCode"] = machine_code;
    args["FieldsToReturn"] = std::to_string(fields_to_return);
    args["SignMethod"] = "1";
    args["v"] = "1";

    std::string url = request_handler.build_url("Activate", args);

    std::string response = request_handler.make_request(url);

    return handle_activate(this->signature_verifier, response);
  }

  // Make an Deactivate request to the SKM Web API
  //
  // Arguments:
  //   token - acces token to use
  //   product_id - the product id
  //   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
  //   machine_code - the machine code, i.e. a string that identifies a device
  //                  for activation.
  //
  // Returns:
  //   A boolean representing if the request was successful or not.
  bool
  deactivate
    ( std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    )
  {
    std::unordered_map<std::string,std::string> args;
    args["token"] = token;
    args["ProductId"] = product_id;
    args["Key"] = key;
    args["MachineCode"] = machine_code;
    args["v"] = "1";

    std::string url = request_handler.build_url("Deactivate", args);

    std::string response = request_handler.make_request(url);

    return handle_deactivate(this->signature_verifier, response);
  }
 
  SignatureVerifier signature_verifier;
  RequestHandler request_handler;
};

} // namespace serialkeymanager_com
