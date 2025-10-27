#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "config_loader/config.h"

TEST_CASE("ConfigLoader Initialization", "[ConfigLoader]") {
    ConfigLoader loader(10);
    REQUIRE(loader.getConfigAt(0).first == "");
}

TEST_CASE("Insert New Config", "[ConfigLoader]") {
    ConfigLoader loader(10);
    json data = {{"key1", "value1"}};
    loader.insertNewConfig("config1.json", data);
    REQUIRE(loader.getConfigAt(0).first == "config1.json");
    REQUIRE(loader.getConfigAt(0).second["key1"] == "value1");
}

TEST_CASE("Load New Config", "[ConfigLoader]") {
    ConfigLoader loader(10);
    json data = {{"key1", "value1"}};
    loader.insertNewConfig("config1.json", data);

    // Save initial config to file
    loader.writeConfig(0);

    // Load the config from file
    loader.loadNewConfig("config1.json", 0);
    REQUIRE(loader.getConfigAt(0).second["key1"] == "value1");
}