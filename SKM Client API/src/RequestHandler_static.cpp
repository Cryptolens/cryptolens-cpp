#include "RequestHandler_static.hpp"

namespace serialkeymanager_com {

RequestHandler_static::RequestHandler_static()
{ }

RequestHandler_static::~RequestHandler_static()
{ }

void
RequestHandler_static::set_response(std::string s)
{
  s_ = std::move(s);
}

std::string
RequestHandler_static::make_request(std::string const& url)
{
  return s_;
}

} // namespace serialkeymanager_com
