#pragma once

#include <string>
#include "utils/env.hpp"

class DBConfig {
  public:
    static const std::string &DB_FILENAME() {
        static const std::string filename = getEnv("DB_FILENAME");
        return filename;
    }
};