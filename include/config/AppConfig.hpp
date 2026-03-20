#pragma once

#include <string>

namespace sip_intercom {

struct AppConfig {
  std::string device_id = "intercom-sim-001";
  std::string log_level = "info";
  int sip_port = 5060;
  int http_port = 8080;
  int ws_port = 8081;
};

} // namespace sip_intercom