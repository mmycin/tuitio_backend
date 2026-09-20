#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class RedisConfig {
  public:
  	inline static const string REDIS_HOST = getEnv("REDIS_HOST");
  	inline static const string REDIS_PORT = getEnv("REDIS_PORT");
};
