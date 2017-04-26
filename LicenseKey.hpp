#pragma once

#include <string>
#include <vector>

#include "ActivationData.hpp"
#include "Customer.hpp"
#include "DataObject.hpp"
#include "optional.hpp"

namespace serialkeymanager_com {

class LicenseKeyChecker;

// This immutable class represents a license key.
//
// The class is constructed using the static factory make().
//
// The check() method can be used to create a LicenseKeyChecker,
// which can be used to check properties of a license key in a
// convenient fashion. I.e.
//
//   if (key.check().has_feature(1).has_not_feature(2)) {
//     DO_SOMETHING();
//   }
//
class LicenseKey {
private:
  LicenseKey() { };

  int           product_id;
  std::uint64_t created;
  std::uint64_t expires;
  int           period;
  bool          block;
  bool          trial_activation;
  std::uint64_t sign_date;
  bool          f1;
  bool          f2;
  bool          f3;
  bool          f4;
  bool          f5;
  bool          f6;
  bool          f7;
  bool          f8;

  optional<int>                         id;
  optional<std::string>                 key;
  optional<std::string>                 notes;
  optional<int>                         global_id;
  optional<Customer>                    customer;
  optional<std::vector<ActivationData>> activated_machines;
  optional<int>                         maxnoofmachines;
  optional<std::string>                 allowed_machines;
  optional<std::vector<DataObject>>     data_objects;
public:
  // Attempt to construct a LicenseKey from a json string
  static optional<LicenseKey> make(std::string const& license_key);

  // Return a LicenseKeyChecker working on this LicenseKey object
  LicenseKeyChecker check() const;

  // Returns the product Id of he license key
  int                get_product_id() const;

  // Returns the date and time the license key was created
  std::uint64_t get_created() const;

  // Returns the date and time the license key expires
  std::uint64_t get_expires() const;

  // Returns the duration of current license cycle eg. 30 days
  int                get_period() const;

  //
  bool               get_block() const;

  // Returns if trial activation is enabled
  bool               get_trial_activation() const;

  // Returns the date the license key was created by the Web API
  std::uint64_t get_sign_date() const;

  // Returns if the license key has feature 1
  bool               get_f1() const;

  // Returns if the license key has feature 2
  bool               get_f2() const;

  // Returns if the license key has feature 3
  bool               get_f3() const;

  // Returns if the license key has feature 4
  bool               get_f4() const;

  // Returns if the license key has feature 5
  bool               get_f5() const;

  // Returns if the license key has feature 6
  bool               get_f6() const;

  // Returns if the license key has feature 7
  bool               get_f7() const;

  // Returns if the license key has feature 8
  bool               get_f8() const;


  // Returns the Id of the license key
  optional<int>                         const& get_id() const;

  // Return the license key string, eg. ABCDE-EFGHI-JKLMO-PQRST
  optional<std::string>                 const& get_key() const;

  // Returns the notes field of the license key
  optional<std::string>                 const& get_notes() const;

  // Returns a unique global identifier for the license key
  optional<int>                         const& get_global_id() const;

  // Returns the customer object assigned to the license key
  optional<Customer>                    const& get_customer() const;

  // Returns the list of activated machines
  optional<std::vector<ActivationData>> const& get_activated_machines() const;

  // Returns the maximum number of machines/devices that may activate this
  // license key.
  optional<int>                         const& get_maxnoofmachines() const;

  // Returns the machine codes of those devices that will be prioritized
  // during activation. Even if the limit is achieved, these will still be
  // activated.
  optional<std::string>                 const& get_allowed_machines() const;

  // Returns the data objects associated with the license key.
  optional<std::vector<DataObject>>     const& get_data_objects() const;
};

} // namespace serialkeymanager_com
