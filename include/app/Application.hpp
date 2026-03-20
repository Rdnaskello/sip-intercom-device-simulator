#pragma once

#include "config/AppConfig.hpp"
#include "logging/Logger.hpp"

namespace sip_intercom {

class Application {
public:
  Application(AppConfig config, Logger &logger);

  int run();

private:
  AppConfig config_;
  Logger &logger_;
};

} // namespace sip_intercom