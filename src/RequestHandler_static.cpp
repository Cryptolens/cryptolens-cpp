#include "RequestHandler_static.hpp"

namespace cryptolens_io {

namespace v20190401 {

/*
 * RequestHandler_static
 */
RequestHandler_static::RequestHandler_static(basic_Error & e)
: response_("")
{ }

#if 0
RequestHandler_static::~RequestHandler_static() { }
#endif

RequestHandler_static::PostBuilder
RequestHandler_static::post_request(basic_Error & e, char const* host, char const* endpoint)
{
  return RequestHandler_static_PostBuilder(this);
}

void
RequestHandler_static::set_response(std::string response)
{
  response_ = std::move(response);
}

/*
 * RequestHandler_static_PostBuilder
 */
RequestHandler_static_PostBuilder::RequestHandler_static_PostBuilder(RequestHandler_static * handler)
: handler_(handler)
{
}

RequestHandler_static_PostBuilder &
RequestHandler_static_PostBuilder::add_argument(basic_Error & e, char const* key, char const* value) {
  return *this;
}

std::string
RequestHandler_static_PostBuilder::make(basic_Error & e)
{
  if (e) { return ""; }

  return handler_->response_;
}

} // namespace v20190401

} // namespace cryptolens_io
