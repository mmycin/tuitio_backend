#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

class AuthManager {
public:
  using Id = uint64_t;

  string make_token(Id id) {
    array<unsigned char, 32> bytes{};
    random_device rd;

    for (auto &b : bytes)
      b = static_cast<unsigned char>(rd());

    static constexpr char hex[] = "0123456789abcdef";

    string token;
    token.reserve(64);

    for (auto b : bytes) {
      token += hex[b >> 4];
      token += hex[b & 0x0f];
    }

    tokens[token] = id;

    return token;
  }

  optional<Id> get_id(string_view token) const {
    auto it = this->tokens.find(string(token));

    if (it == tokens.end())
      return nullopt;
    return it->second;
  }

private:
  unordered_map<string, Id> tokens;
};
