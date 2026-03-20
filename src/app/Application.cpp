#include "app/Application.hpp"

#include <sstream>
#include <utility>

namespace sip_intercom {

Application::Application(AppConfig config, Logger &logger)
    : config_(std::move(config)), logger_(logger) {}

int Application::run() {
  logger_.info("app", "Starting SIP Intercom Device Simulator skeleton.");

  std::ostringstream config_summary;
  config_summary << "Loaded configuration: " << "device_id="
                 << config_.device_id << ", sip_port=" << config_.sip_port
                 << ", http_port=" << config_.http_port
                 << ", ws_port=" << config_.ws_port
                 << ", log_level=" << config_.log_level;

  logger_.info("app", config_summary.str());
  logger_.info("app", "Core runtime modules are not implemented yet.");
  logger_.info("app", "Application skeleton finished successfully.");

  return 0;
}

} // namespace sip_intercom