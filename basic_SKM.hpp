#pragma once

#include <string>
#include <unordered_map>

#include "optional.hpp"

#include "json.hpp"

#include "RawLicenseKey.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyChecker.hpp"

namespace serialkeymanager_com {

using json = nlohmann::json;
using json_exception = nlohmann::detail::exception;

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

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  json j;
  try {
    j = json::parse(response);
  } catch (json_exception & e) {
    return nullopt;
  }

  return RawLicenseKey::make(signature_verifier, j["licenseKey"], j["signature"]);
}

// TODO: Do we need to template this? Since we currently dont check signature anyway...
template<typename SignatureVerifier>
bool
handle_deactivate
  ( SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  json j;
  try {
  j = json::parse(response);
  } catch (json_exception & e) {
    return false;
  }

  return j["Result"] == 0;
}

template<typename RequestHandler, typename SignatureVerifier>
class basic_SKM
{
public:
  basic_SKM() { }

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
    args["FieldsToReturn"] = fields_to_return;
    args["SignMethod"] = "1";
    args["v"] = 1;

    std::string url = request_handler.build_url("Activate", args);

    std::string response = request_handler.make_request(url);

    return handle_activate(this->signature_verifier, response);
  }

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
    args["v"] = 1;

    std::string url = request_handler.build_url("Deactivate", args);

    std::string response = request_handler.make_request(url);

    return handle_deactivate(this->signature_verifier, response);
  }

  SignatureVerifier signature_verifier;
  RequestHandler request_handler;
};

} // namespace serialkeymanager_com
