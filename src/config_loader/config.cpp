#include "config_loader/config.h"





void ConfigLoader::loadNewConfig(const std::string& path, int intPos) {


    if (intPos < 0 || intPos >= jsonList.size()) {
        std::cerr << "Invalid index: " << intPos << std::endl;
        return;
    }

    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }

    json data;
    try {
        f >> data;
    } catch (json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return;
    }

    jsonList[intPos] = std::make_pair(path, data);
}

int ConfigLoader::getSize(){
    return jsonList.size();
}

void ConfigLoader::writeConfig(int intPos) {
    if (intPos < 0 || intPos >= jsonList.size()) {
        std::cerr << "Invalid index: " << intPos << std::endl;
        return;
    }

    std::ofstream o(jsonList[intPos].first);
    if (!o.is_open()) {
        std::cerr << "Could not open file: " << jsonList[intPos].first << std::endl;
        return;
    }

    o << std::setw(4) << jsonList[intPos].second << std::endl;
}

std::pair<std::string, json> ConfigLoader::getConfigAt(int intPos) const {
    if (intPos < 0 || intPos >= jsonList.size()) {
        std::cerr << "Invalid index: " << intPos << std::endl;
        return {};
    }
    return jsonList[intPos];
}

json ConfigLoader::getConfig() {
    // Dummy implementation, assuming it returns some json object
    return json();
}

void ConfigLoader::changeConfigAt(int key) {
    // Dummy implementation, assuming it changes some configuration
}

void ConfigLoader::insertNewConfig(const std::string& path, const json& jsonData) {
    for (const auto& item : jsonList) {
        if (item.first == path) {
            std::cerr << "This config path is already opened: " << path << std::endl;
            return;
        }
    }

    jsonList.emplace_back(path, jsonData);
}

// Overload for [] to make accesing less of a pain (this schould be the go to method)
std::pair<std::string, json>& ConfigLoader::operator[](int index) {
    if (index < 0 || index >= jsonList.size()) {
        throw std::out_of_range("Index out of range");
    }
    return jsonList[index];
}

const std::pair<std::string, json>& ConfigLoader::operator[](int index) const {
    if (index < 0 || index >= jsonList.size()) {
        throw std::out_of_range("Index out of range");
    }
    return jsonList[index];
}