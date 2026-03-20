#include "config/ConfigLoader.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace sip_intercom {
namespace {

std::string readFileContents(const std::string &path) {
  std::ifstream input(path);
  if (!input.is_open()) {
    throw std::runtime_error("Failed to open config file: " + path);
  }

  std::ostringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

std::string extractStringField(const std::string &content,
                               const std::string &fieldName) {
  const std::regex pattern("\"" + fieldName + "\"\\s*:\\s*\"([^\"]+)\"");
  std::smatch match;

  if (!std::regex_search(content, match, pattern)) {
    throw std::runtime_error("Missing or invalid string field in config: " +
                             fieldName);
  }

  return match[1].str();
}

int extractIntField(const std::string &content, const std::string &fieldName) {
  const std::regex pattern("\"" + fieldName + "\"\\s*:\\s*(\\d+)");
  std::smatch match;

  if (!std::regex_search(content, match, pattern)) {
    throw std::runtime_error("Missing or invalid integer field in config: " +
                             fieldName);
  }

  return std::stoi(match[1].str());
}

bool isValidLogLevel(const std::string &value) {
  return value == "debug" || value == "info" || value == "warn" ||
         value == "error";
}

bool isValidPort(int port) { return port >= 1 && port <= 65535; }

void validateConfig(const AppConfig &config) {
  if (config.device_id.empty()) {
    throw std::runtime_error(
        "Config validation failed: device_id must not be empty.");
  }

  if (!isValidLogLevel(config.log_level)) {
    throw std::runtime_error("Config validation failed: log_level must be one "
                             "of debug, info, warn, error.");
  }

  if (!isValidPort(config.sip_port)) {
    throw std::runtime_error(
        "Config validation failed: sip_port must be between 1 and 65535.");
  }

  if (!isValidPort(config.http_port)) {
    throw std::runtime_error(
        "Config validation failed: http_port must be between 1 and 65535.");
  }

  if (!isValidPort(config.ws_port)) {
    throw std::runtime_error(
        "Config validation failed: ws_port must be between 1 and 65535.");
  }
}

} // namespace

AppConfig ConfigLoader::load(const std::string &path) const {
  const std::string content = readFileContents(path);

  AppConfig config;
  config.device_id = extractStringField(content, "device_id");
  config.log_level = extractStringField(content, "log_level");
  config.sip_port = extractIntField(content, "sip_port");
  config.http_port = extractIntField(content, "http_port");
  config.ws_port = extractIntField(content, "ws_port");

  validateConfig(config);
  return config;
}

} // namespace sip_intercom