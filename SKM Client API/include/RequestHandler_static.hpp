#pragma once

#include <string>

namespace serialkeymanager_com {

// A request handler with a settable string that is returned. Used for testing
class RequestHandler_static
{
public:
  RequestHandler_static();
  ~RequestHandler_static();

  void
  set_response(std::string s);

  std::string
  make_request(std::string const& url);

  template<typename Map>
  std::string
  build_url(char const* method, Map const& map)
  {
    return std::string("");
  }

private:
  std::string s_;
};

} // namespace serialkeymanager_com
