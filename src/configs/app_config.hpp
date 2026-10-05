#pragma once

#include <string>
#include "utils/env.hpp"

class AppConfig {
  public:
    static const std::string &APP_ENV() {
        static const std::string env = getEnv("APP_ENV");
        return env;
    }
    static const std::string &APP_HOST() {
        static const std::string host = getEnv("APP_HOST");
        return host;
    }
    static int APP_PORT() {
        std::string port_str = getEnv("APP_PORT");
        if (port_str.empty()) {
            return 5000;
        }
        return std::stoi(port_str);
    }
};

