#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class AppConfig {
  public:
  	inline static const string APP_ENV = getEnv("APP_ENV");
};
