#pragma once

#include <string>
#include "utils/env.hpp"

class SecretConfig {
  public:
    static const std::string &SECRET_KEY() {
        static const std::string key = getEnv("SECRET_KEY");
        return key;
    }
};