/** \mainpage Main page
 *
 * This repository contains the official C++ API for interacting with the Serialkeymanager.com
 * web API.
 *
 * The C++ API currently supports a subset of all methods available from the web API, more
 * precisly, activation and deactivation of license keys are currently supported.
 * The C++ library is currently under development, if you have any other needs, dont hesitate
 * to contact us.
 *
 * TODO:
 *   - Clarify what has to be checked manually and what the library takes care of
 *   - Info about RequestHandlers and SignatureVerifiers.
 *
 * Example usage:
 *
 *     SKM skm_handle;
 *     skm::Error e;
 *     optional<RawLicenseKey> raw_license_key = skm_handle.activate(e, "AAAA-BBBB-...", ...);
 *     optional<LicenseKey> license_key= skm::LicenseKey::make(e, raw_license_key);
 *     if (e) { handle_error(e); }
 *     ...
 *
 * The RawLicenseKey can be used to store the key for offline activation at subsequent attempts:
 *
 *     SKM skm_handle;
 *     skm::Error e;
 *     optional<RawLicenseKey> raw_license_key = skm_handle.activate(e, "AAAA-BBBB-...", ...);
 *     if (e) { handle_error(e); }
 *     file << raw_license_key->get_license() << "-" << raw_license_key->get_signature() << std::endl;
 *
 * and then when offline the activation can be verified via:
 *
 *     skm::Error e;
 *     std::string s << file;
 *     std::string license = ...
 *     std::string signature = ...
 *     optional<RawLicenseKey> raw_license_key = skm::RawLicenseKey::make(e, license, signature);
 *     if (e) { handle_error(e); }
 */
