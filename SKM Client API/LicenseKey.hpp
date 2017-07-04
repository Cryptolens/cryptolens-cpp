#pragma once

#include <string>
#include <vector>

#include "ActivationData.hpp"
#include "Customer.hpp"
#include "DataObject.hpp"
#include "RawLicenseKey.hpp"
#include "optional.hpp"

namespace serialkeymanager_com {

class LicenseKeyChecker;

/**
 * This immutable class represents a license key.
 *
 * The class is constructed using the static factory make().
 *
 * The check() method can be used to create a LicenseKeyChecker,
 * which can be used to check properties of a license key in a
 * convenient fashion. I.e.
 *
 *   if (key.check().has_feature(1).has_not_feature(2)) {
 *     DO_SOMETHING();
 *   }
 */
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
  static optional<LicenseKey> make(Error & e, RawLicenseKey const& raw_license_key);
  static optional<LicenseKey> make(Error & e, optional<RawLicenseKey> const& raw_license_key);
  static optional<LicenseKey> make_unsafe(Error & e, std::string const& license_key);

  LicenseKeyChecker check() const;

  int           get_product_id() const;
  std::uint64_t get_created() const;
  std::uint64_t get_expires() const;
  int           get_period() const;
  bool          get_block() const;
  bool          get_trial_activation() const;
  std::uint64_t get_sign_date() const;
  bool          get_f1() const;
  bool          get_f2() const;
  bool          get_f3() const;
  bool          get_f4() const;
  bool          get_f5() const;
  bool          get_f6() const;
  bool          get_f7() const;
  bool          get_f8() const;

  optional<int>                         const& get_id() const;
  optional<std::string>                 const& get_key() const;
  optional<std::string>                 const& get_notes() const;
  optional<int>                         const& get_global_id() const;
  optional<Customer>                    const& get_customer() const;
  optional<std::vector<ActivationData>> const& get_activated_machines() const;
  optional<int>                         const& get_maxnoofmachines() const;
  optional<std::string>                 const& get_allowed_machines() const;
  optional<std::vector<DataObject>>     const& get_data_objects() const;
};

} // namespace serialkeymanager_com
