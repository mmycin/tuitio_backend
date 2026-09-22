#pragma once

#include <string>
#include "utils/env.hpp"

using namespace std;

class DBConfig {
	public:
		inline static const string DB_FILENAME = getEnv("DB_FILENAME");
};