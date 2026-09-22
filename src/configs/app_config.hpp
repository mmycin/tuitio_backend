#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class AppConfig {
  public:
  	inline static const string APP_ENV = getEnv("APP_ENV");
  	inline static const string APP_HOST = getEnv("APP_HOST");
  	inline static const int APP_PORT = stoi(getEnv("APP_PORT"));
};
