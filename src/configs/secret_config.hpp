#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class SecretConfig {
	public:
		inline static const string SECRET_KEY = getEnv("SECRET_KEY");
};