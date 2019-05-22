#include "basic_Cryptolens.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

int
activate_parse_server_error_message(char const* server_response)
{
  using namespace errors;

  if (server_response == NULL) { return Main::UNKNOWN_SERVER_REPLY; }

  if (0 == std::strcmp(server_response, "Unable to authenticate.")) {
    return Main::INVALID_ACCESS_TOKEN;
  }

  if (0 == std::strcmp(server_response, "Access denied.")) {
    return Main::ACCESS_DENIED;
  }

  if (0 == std::strcmp(server_response, "The input parameters were incorrect.")) {
    return Main::INCORRECT_INPUT_PARAMETER;
  }

  if (0 == std::strcmp(server_response, "Could not find the product.")) {
    return Main::PRODUCT_NOT_FOUND;
  }

  if (0 == std::strcmp(server_response, "Could not find the key.")) {
    return Main::KEY_NOT_FOUND;
  }

  if (0 == std::strcmp(server_response, "The key is blocked and cannot be accessed.")) {
    return Main::KEY_BLOCKED;
  }

  if (0 == std::strcmp(server_response, "Cannot activate the new device as the limit has been reached.")) {
    return Main::DEVICE_LIMIT_REACHED;
  }

  return Main::UNKNOWN_SERVER_REPLY;
}

} // namespace internal

} // namespace v20190401

} // namespace cryptolens_io
