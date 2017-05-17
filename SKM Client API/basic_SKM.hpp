#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <unordered_map>

#include "optional.hpp"

#include "ArduinoJson.hpp"

#include "RawLicenseKey.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyChecker.hpp"
#include "api.hpp"

namespace serialkeymanager_com {

using namespace ArduinoJson;

class ActivateError;

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

template<typename RequestHandler>
std::string
make_activate_request
  ( RequestHandler & request_handler
  , std::string const& token
  , std::string const& product_id
  , std::string const& key
  , std::string const& machine_code
  , int fields_to_return = 0
  )
{
  std::unordered_map<std::string,std::string> args;
  args["token"] = token;
  args["ProductId"] = product_id;
  args["Key"] = key;
  args["Sign"] = "true";
  args["MachineCode"] = machine_code;
  // Fix since to_string is not available everywhere
  //args["FieldsToReturn"] = std::to_string(fields_to_return);
  std::ostringstream stm; stm << fields_to_return;
  args["FieldsToReturn"] = stm.str();
  args["SignMethod"] = "1";
  args["v"] = "1";

  std::string url = request_handler.build_url("Activate", args);

  return request_handler.make_request(url);
}

// Function for handling a response to an Activate request from
// the SKM Web API
template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  try {
    return make_optional(
	     handle_activate_exn( signature_verifier
		                , response)
	   );
  } catch (ActivateError const& e) {
    return nullopt;
  }
}

template<typename SignatureVerifier>
RawLicenseKey
handle_activate_exn
  ( experimental_v1 experimental
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(response);

  if (!j.success()) { throw ActivateError(NULL); }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      throw ActivateError(NULL);
    }

    throw ActivateError(j["message"].as<char const*>());
  }

  if (!j["licenseKey"].is<char const*>() || j["licenseKey"].as<char const*>() == NULL) {
    throw ActivateError(NULL);
  }

  if (!j["signature"].is<char const*>() || j["signature"].as<char const*>() == NULL) {
    throw ActivateError(NULL);
  }

  optional<RawLicenseKey> raw = RawLicenseKey::make(
             signature_verifier
           , j["licenseKey"].as<char const*>()
           , j["signature"].as<char const*>()
	   );

  if (raw) {
    return *raw;
  } else {
    throw ActivateError(NULL);
  }
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

  if (!j["result"].is<int>()) {
    return false;
  }

  return j["result"].as<int>() == 0;
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
    std::string response =
      make_activate_request
        ( this->request_handler
        , token
        , product_id
        , key
        , machine_code
        , fields_to_return
      );

    return handle_activate(this->signature_verifier, response);
  }

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
  //   An optional with a RawLicenseKey, if the request is successful this always
  //   contains a value. If the request is unsuecessful an ActivateError is thrown.
  RawLicenseKey
  activate_exn
    ( experimental_v1 experimental
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    )
  {
    std::string response =
      make_activate_request
        ( this->request_handler
        , token
        , product_id
        , key
        , machine_code
        , fields_to_return
      );

    return handle_activate_exn(experimental, this->signature_verifier, response);
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

class ActivateError : public std::exception {
private:
  int reason;

public:
  static const int UNKNOWN_SERVER_REPLY      = 0;
  static const int INVALID_ACCESS_TOKEN      = 1;
  static const int ACCESS_DENIED             = 2;
  static const int INCORRECT_INPUT_PARAMETER = 3;
  static const int PRODUCT_NOT_FOUND         = 4;
  static const int KEY_NOT_FOUND             = 5;
  static const int KEY_BLOCKED               = 6;
  static const int DEVICE_LIMIT_REACHED      = 7;

  ActivateError(const char *server_response)
  {
    reason = UNKNOWN_SERVER_REPLY;
    if (server_response == NULL) { return; }

    if (0 == std::strcmp(server_response, "Unable to authenticate.")) {
      reason = INVALID_ACCESS_TOKEN;
    }

    if (0 == std::strcmp(server_response, "Access denied.")) {
      reason = ACCESS_DENIED;
    }

    if (0 == std::strcmp(server_response, "The input parameters were incorrect.")) {
      reason = INCORRECT_INPUT_PARAMETER;
    }

    if (0 == std::strcmp(server_response, "Could not find the product.")) {
      reason = PRODUCT_NOT_FOUND;
    }

    if (0 == std::strcmp(server_response, "Could not find the key.")) {
      reason = KEY_NOT_FOUND;
    }

    if (0 == std::strcmp(server_response, "The key is blocked and cannot be accessed.")) {
      reason = KEY_BLOCKED;
    }

    if (0 == std::strcmp(server_response, "Cannot activate the new device as the limit has been reached.")) {
      reason = DEVICE_LIMIT_REACHED;
    }
  }

  int get_reason() { return reason; }

  const char * what() {
    switch (reason) {
    case INVALID_ACCESS_TOKEN:
    return "Invalid access token.";

    case UNKNOWN_SERVER_REPLY:
    return "Recieved unknown reply from the server.";

    case ACCESS_DENIED:
    return "Access denied.";

    case INCORRECT_INPUT_PARAMETER:
    return "The input parameters were incorrect.";

    case PRODUCT_NOT_FOUND:
    return "Could not find the product." ;

    case KEY_NOT_FOUND:
    return "Could not find the key.";

    case KEY_BLOCKED:
    return "The key is blocked and cannot be accessed.";

    case DEVICE_LIMIT_REACHED:
    return "Cannot activate the new device as the limit has been reached.";

    default:
    return "Unknown error.";
    }
  }
};

} // namespace serialkeymanager_com
