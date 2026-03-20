#include "logging/Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace sip_intercom {
namespace {

std::string makeTimestamp() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  std::tm local_time{};
#if defined(_WIN32)
  localtime_s(&local_time, &now_time);
#else
  localtime_r(&now_time, &local_time);
#endif

  std::ostringstream stream;
  stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
  return stream.str();
}

int toSeverity(LogLevel level) {
  switch (level) {
  case LogLevel::Debug:
    return 0;
  case LogLevel::Info:
    return 1;
  case LogLevel::Warn:
    return 2;
  case LogLevel::Error:
    return 3;
  }

  return 1;
}

std::string colorizeLevel(LogLevel level, const std::string &levelText) {
  switch (level) {
  case LogLevel::Warn:
    return "\033[33m" + levelText + "\033[0m";
  case LogLevel::Error:
    return "\033[31m" + levelText + "\033[0m";
  case LogLevel::Info:
  case LogLevel::Debug:
  default:
    return levelText;
  }
}

} // namespace

void Logger::initialize(LogLevel level) {
  current_level_ = level;
  initialized_ = true;
}

void Logger::debug(const std::string &component,
                   const std::string &message) const {
  log(LogLevel::Debug, component, message);
}

void Logger::info(const std::string &component,
                  const std::string &message) const {
  log(LogLevel::Info, component, message);
}

void Logger::warn(const std::string &component,
                  const std::string &message) const {
  log(LogLevel::Warn, component, message);
}

void Logger::error(const std::string &component,
                   const std::string &message) const {
  log(LogLevel::Error, component, message);
}

LogLevel Logger::fromString(const std::string &value) {
  if (value == "debug") {
    return LogLevel::Debug;
  }

  if (value == "info") {
    return LogLevel::Info;
  }

  if (value == "warn") {
    return LogLevel::Warn;
  }

  if (value == "error") {
    return LogLevel::Error;
  }

  throw std::runtime_error("Invalid log level: " + value);
}

void Logger::log(LogLevel level, const std::string &component,
                 const std::string &message) const {
  if (!initialized_ || !shouldLog(level)) {
    return;
  }

  std::ostream &output = (level == LogLevel::Error) ? std::cerr : std::cout;
  const std::string level_text = toString(level);
  const std::string colored_level_text = colorizeLevel(level, level_text);

  output << makeTimestamp() << " [" << colored_level_text << "]" << " ["
         << component << "] " << message << '\n';
}

bool Logger::shouldLog(LogLevel level) const {
  return toSeverity(level) >= toSeverity(current_level_);
}

std::string Logger::toString(LogLevel level) {
  switch (level) {
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warn:
    return "WARN";
  case LogLevel::Error:
    return "ERROR";
  }

  return "INFO";
}

} // namespace sip_intercom