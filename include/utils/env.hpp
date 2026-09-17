#pragma once

#include <stdlib.h>
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdlib>
#include <string>

using namespace std;

inline string getEnv(const string &key) {
  const char *value = getenv(key.c_str());
  if (value == nullptr)
    return {};

  return string(value);
}
