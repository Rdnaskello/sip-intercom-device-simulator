#pragma once

#include <string>

namespace sip_intercom {

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
public:
  void initialize(LogLevel level);

  void debug(const std::string &component, const std::string &message) const;
  void info(const std::string &component, const std::string &message) const;
  void warn(const std::string &component, const std::string &message) const;
  void error(const std::string &component, const std::string &message) const;

  static LogLevel fromString(const std::string &value);

private:
  void log(LogLevel level, const std::string &component,
           const std::string &message) const;
  bool shouldLog(LogLevel level) const;
  static std::string toString(LogLevel level);

  LogLevel current_level_ = LogLevel::Info;
  bool initialized_ = false;
};

} // namespace sip_intercom