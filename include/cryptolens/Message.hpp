#pragma once

#include <cstdint>
#include <string>

namespace cryptolens_io {

namespace v20190401 {

// An immutable class representing a message sent using the Messaging API
class Message {
private:
  std::string message_;
  std::uint64_t time_;
public:
  Message
    ( std::string message
    , std::uint64_t time
    )
  : message_(std::move(message))
  , time_(time)
  { }

  // Returns the message
  std::string const& get_message() const { return message_; }

  // Returns the time the message was sent
  std::uint64_t get_time() const { return time_; }
};

} // namespace v20190401

namespace latest {

using ::cryptolens_io::v20190401::ActivationData;

} // namespace latest

} // namespace cryptolens_io
