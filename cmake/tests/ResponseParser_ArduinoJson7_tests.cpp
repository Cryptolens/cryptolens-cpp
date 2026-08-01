#include <iostream>
#include <string>

#include <cryptolens/Error.hpp>
#include <cryptolens/ResponseParser_ArduinoJson7.hpp>

namespace cryptolens = cryptolens_io::latest;

namespace {

int failures = 0;

void expect(bool condition, char const* message)
{
  if (!condition) {
    std::cerr << message << std::endl;
    ++failures;
  }
}

std::string license_json(std::string const& f5_member)
{
  return std::string(
      "{"
      "\"ProductId\":1,"
      "\"Created\":1,"
      "\"Expires\":1,"
      "\"Period\":-7,"
      "\"Block\":false,"
      "\"TrialActivation\":false,"
      "\"SignDate\":1,"
      "\"F1\":false,"
      "\"F2\":false,"
      "\"F3\":false,"
      "\"F4\":false,")
    + f5_member
    + "\"F6\":false,"
      "\"F7\":false,"
      "\"F8\":false,"
      "\"DataObjects\":[{"
        "\"Id\":1,"
        "\"Name\":\"quota\","
        "\"StringValue\":\"\","
        "\"IntValue\":-9"
      "}]"
      "}";
}

void signed_integer_values_are_preserved()
{
  cryptolens::Error error;
  cryptolens::ResponseParser_ArduinoJson7 parser(error);
  auto license = parser.make_license_key_information_unsafe(
      error, license_json("\"F5\":true,"));

  expect(!error, "A license with signed integer values should parse.");
  expect(static_cast<bool>(license), "The parsed license should be present.");
  if (!license) {
    return;
  }

  expect(license->get_period() == -7, "The negative Period should be preserved.");
  expect(license->get_f5(), "F5 should be parsed as true.");

  auto const& data_objects = license->get_data_objects();
  expect(static_cast<bool>(data_objects), "DataObjects should be present.");
  if (!data_objects) {
    return;
  }

  expect(data_objects->size() == 1, "Exactly one DataObject should be parsed.");
  if (data_objects->size() == 1) {
    expect(
        data_objects->front().get_int_value() == -9,
        "The negative DataObject IntValue should be preserved.");
  }
}

void invalid_f5_is_rejected(std::string const& f5_member, char const* message)
{
  cryptolens::Error error;
  cryptolens::ResponseParser_ArduinoJson7 parser(error);
  auto license =
      parser.make_license_key_information_unsafe(error, license_json(f5_member));

  expect(static_cast<bool>(error), message);
  expect(!license, "An invalid F5 value should not produce a license.");
  expect(
      error.get_subsystem() == cryptolens::errors::Subsystem::Json,
      "An invalid F5 value should report the JSON subsystem.");
}

} // namespace

int main()
{
  signed_integer_values_are_preserved();
  invalid_f5_is_rejected("", "A missing F5 field should be rejected.");
  invalid_f5_is_rejected(
      "\"F5\":\"true\",", "A non-Boolean F5 field should be rejected.");

  return failures == 0 ? 0 : 1;
}
