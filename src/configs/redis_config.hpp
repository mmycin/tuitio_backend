#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class RedisConfig {
  public:
    static inline const string &REDIS_HOST() {
        static const string value = getEnv("REDIS_HOST");
        return value;
    }
    static inline const string &REDIS_PORT() {
        static const string value = getEnv("REDIS_PORT");
        return value;
    }
};
