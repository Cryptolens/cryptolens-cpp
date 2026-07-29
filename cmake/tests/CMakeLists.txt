add_executable (
  cryptolens_response_parser_tests
  "ResponseParser_ArduinoJson7_tests.cpp"
)

target_link_libraries (cryptolens_response_parser_tests PRIVATE cryptolens)

add_test (
  NAME cryptolens_response_parser_tests
  COMMAND cryptolens_response_parser_tests
)
