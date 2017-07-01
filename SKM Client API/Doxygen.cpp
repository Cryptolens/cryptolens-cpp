/** \mainpage Main page
 *
 * This repository contains the official C++ API for interacting with the Serialkeymanager.com
 * web API.
 *
 * The C++ API currently supports a subset of the methods available via the web API, more
 * precisely, activation and deactivation of license keys are currently supported.
 * The C++ library is currently under development, if you have any other needs, dont hesitate
 * to contact us.
 *
 * TODO:
 *   - Clarify what has to be checked manually and what the library takes care of
 *
 * Example usage
 * =============
 *
 * The library is built to be flexible in the way the actual HTTPS requests are made to the
 * serialkeymanager.com web API. The examples below instantiate a handle which is responsible
 * for making the HTTPS requests. This handle is parameterized by several policy classes which
 * can be used to change how HTTPS requests are performed. In case this is too heavy weight
 * it is also possible to sidestep this entierly and manually make the HTTPS request and
 * simply provide the response as a string.
 *
 * Basic usage:
 *
 *     SKM skm_handle;
 *     skm::Error e;
 *     optional<RawLicenseKey> raw_license_key = skm_handle.activate(e, "AAAA-BBBB-...", ...);
 *     optional<LicenseKey> license_key= skm::LicenseKey::make(e, raw_license_key);
 *     if (e) { handle_error(e); }
 *
 *     /* CHECK KEY IS STILL VALID */
 *
 *     if (license_key->check()-has_feature(1)) { std::cout << "Hello, Mr President!" << std::endl; }
 *     else                                     { std::cout << "Welcome!" << std::endl; }
 *
 * As can be seen above two types are used to represent license keys, the RawLicenseKey and
 * LicenseKey. All queries regarding properties of a license key need to be performed on the
 * LicenseKey object. The purpuse of the RawLicenseKey class is to enable offline activation,
 * where a previous activation can be saved to disk and then later loaded to check the
 * validity of a license key without needing to make any HTTP requests. Note that when the
 * RawLicenseKey is constructed a cryptographic check is performed to make sure the license
 * key has not been tampered with.
 *
 * Offline activation example:
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
 *
 * Error handling
 * ==============
 *
 * The library uses an exceptionless design and return values use optionals where they can be missing.
 * All functions that can fail (not really true, due to design decision described below) also accept as the first argument an instance of skm::Error which
 * is used for reporting more detailed error information.
 *
 * Furthermore, an additional design decision is used to with regards to error handling. If an
 * error occurs, the following functions that take the same error object are turned into noops.
 * Consider the following code:
 *
 *     skm::Error e;
 *     f1(e);
 *     f2(e);
 *
 * If an error occurs during the call to f1(), f2() will not be performed. This allows writing
 * several function calls after each other without having to check for errors after
 * every single call.
 *
 * Handle class
 * ============
 *
 * The SKM class above is really an alias for the basic_SKM class, which takes two type parameters
 * represnting a class for dealing with HTTP requests and one for checking cryptographic signatures.
 * Currently one version of each is provided, the RequestHandler_curl class for making HTTPS requests
 * built on top of the Curl library and the SignatureVerifier_OpenSSL for checking cryptographic
 * signatures built on top of the OpenSSL/LibreSSL/Something else libraries.
 *
 *   using SKM = basic_SKM<RequestHandler_curl, SignatureVerifier_OpenSSL>;
 *
 * Manual HTTPS requests
 * =====================
 *
 */
