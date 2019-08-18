#include "RawLicenseKey.hpp"

namespace cryptolens_io {

namespace v20190401 {

std::string const&
RawLicenseKey::get_base64_license() const
{
  return base64_license_;
}

std::string const&
RawLicenseKey::get_signature() const
{
  return signature_;
}

std::string const&
RawLicenseKey::get_license() const
{
  return license_;
}

} // namespace v20190401

} // namespace cryptolens_io
