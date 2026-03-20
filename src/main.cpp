#include "app/Application.hpp"
#include "config/ConfigLoader.hpp"
#include "logging/Logger.hpp"

#include <exception>
#include <stdexcept>
#include <string>

namespace {

std::string resolveConfigPath(int argc, char *argv[]) {
  const std::string default_path = "configs/app_config.json";

  if (argc == 1) {
    return default_path;
  }

  if (argc == 3 && std::string(argv[1]) == "--config") {
    return argv[2];
  }

  throw std::runtime_error(
      "Usage: ./sip_intercom_device_simulator [--config <path>]");
}

} // namespace

int main(int argc, char *argv[]) {
  sip_intercom::Logger logger;

  try {
    const std::string config_path = resolveConfigPath(argc, argv);

    logger.initialize(sip_intercom::LogLevel::Info);
    logger.info("main", "Starting application bootstrap.");
    logger.info("main", "Loading configuration from: " + config_path);

    sip_intercom::ConfigLoader config_loader;
    const sip_intercom::AppConfig config = config_loader.load(config_path);

    logger.initialize(sip_intercom::Logger::fromString(config.log_level));
    logger.info("main", "Configuration loaded successfully.");

    sip_intercom::Application app(config, logger);
    return app.run();
  } catch (const std::exception &exception) {
    logger.initialize(sip_intercom::LogLevel::Error);
    logger.error("main", std::string("Fatal error: ") + exception.what());
    return 1;
  }
}