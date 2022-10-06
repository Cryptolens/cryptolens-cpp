#pragma once

#include <string>

#include "basic_Error.hpp"
#include "RequestHandler_v20190401_to_v20180502.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace RequestHandler_static {
} // namespace RequestHandler_static

} // namespace errors

class RequestHandler_static;

class RequestHandler_static_PostBuilder {
public:
  RequestHandler_static_PostBuilder(RequestHandler_static * handler);

  RequestHandler_static_PostBuilder &
  add_argument(basic_Error & e, char const* key, char const* value);

  std::string
  make(basic_Error & e);

private:
  RequestHandler_static * handler_;
};

/**
 */
class RequestHandler_static
{
public:
  explicit RequestHandler_static(basic_Error & e);
  RequestHandler_static(RequestHandler_static const&) = delete;
  RequestHandler_static(RequestHandler_static &&) = delete;
  void operator=(RequestHandler_static const&) = delete;
  void operator=(RequestHandler_static &&) = delete;
  //~RequestHandler_static();

  using PostBuilder = RequestHandler_static_PostBuilder;

  PostBuilder
  post_request(basic_Error & e, char const* host, char const* endpoint);

  void
  set_response(std::string s);
private:
  std::string response_;

  friend class RequestHandler_static_PostBuilder;
};

} // namespace v20190401

namespace latest {

namespace errors {

namespace RequestHandler_static = ::cryptolens_io::v20190401::errors::RequestHandler_static;

} // namespace errors

using RequestHandler_static = ::cryptolens_io::v20190401::RequestHandler_static;

} // namespace latest

namespace v20180502 {

namespace errors {

namespace RequestHandler_static {
} // namespace RequestHandler_static

} // namespace errors

using RequestHandler_static = ::cryptolens_io::internal::RequestHandler_v20190401_to_v20180502<::cryptolens_io::v20190401::RequestHandler_static>;

} // namespace v20180502

} // namespace cryptolens_io
