#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::set()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("integer") {
    doc.set(42);

    REQUIRE(doc.as<std::string>() == "42");
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("const char*") {
    doc.set("example");

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("std::string") {
    doc.set("example"_s);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("char*") {
    char value[] = "example";
    doc.set(value);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Arduino String") {
    doc.set(String("example"));

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Flash string") {
    doc.set(F("example"));

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "example");

    doc.set(vla);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }
#endif
}
