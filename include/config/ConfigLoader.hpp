#pragma once

#include <string>

#include "config/AppConfig.hpp"

namespace sip_intercom {

class ConfigLoader {
public:
  AppConfig load(const std::string &path) const;
};

} // namespace sip_intercom