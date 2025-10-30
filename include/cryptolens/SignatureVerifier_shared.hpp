#pragma once

#include <string>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

template<typename SignatureVerifier>
void
set_public_key_xml(basic_Error & e, SignatureVerifier & signature_verifier, std::string const& key_xml)
{
  if (e) { return; }

  using size_type = std::string::size_type;

  size_type m_s = key_xml.find("<Modulus>");
  size_type m_e = key_xml.find("</Modulus>");

  size_type e_s = key_xml.find("<Exponent>");
  size_type e_e = key_xml.find("</Exponent>");

  if (m_s == std::string::npos || m_e == std::string::npos || m_e < m_s + 9 ||
      e_s == std::string::npos || e_e == std::string::npos || e_e < e_s + 10)
  {
    e.set(api::main(), errors::Subsystem::Base64, __LINE__); // TODO: Should this really be the Base64 subsystem?
    return;
  }

  size_type m_start  = m_s + 9;
  size_type m_length = m_e - m_s - 9;

  size_type e_start  = e_s + 10;
  size_type e_length = e_e - e_s - 10;

  std::string modulus_base64  = key_xml.substr(m_start, m_length);
  std::string exponent_base64 = key_xml.substr(e_start, e_length);

  signature_verifier.set_public_key_base64(e, modulus_base64, exponent_base64);
}

} // namespace internal

} // namespace v20190401

} // namespace cryptolens_io

