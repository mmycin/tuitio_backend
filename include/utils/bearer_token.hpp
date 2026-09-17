#pragma once

#include <aes_cpp/aes.hpp>
#include <cstdint>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <vector>
#include "configs/secret_config.hpp"

using namespace std;

// ---------------------------------------------------------------------------
// Token format (URL-safe, dot-separated hex):
//   <12-byte IV hex>.<ciphertext hex>.<16-byte GCM tag hex>
//
// Payload encrypted: decimal string of the user Id
// Key: SECRET_KEY env var, zero-padded or truncated to 32 bytes (AES-256)
// ---------------------------------------------------------------------------

class AuthManager {
public:
  using Id = uint64_t;

  // Generate a stateless AES-256-GCM token encoding the user id.
  string make_token(Id id) const {
    auto key = load_key();
    auto iv  = random_iv();

    const string payload = to_string(id);
    vector<unsigned char> plain(payload.begin(), payload.end());

    aes_cpp::AES aes(aes_cpp::AESKeyLength::AES_256);
    vector<unsigned char> tag;
    vector<unsigned char> cipher =
        aes.EncryptGCM(plain, key, iv, /*aad=*/{}, tag);

    return to_hex(iv) + "." + to_hex(cipher) + "." + to_hex(tag);
  }

  // Validate and decode a token. Returns nullopt on any failure.
  optional<Id> get_id(string_view token) const {
    // Split on '.'
    auto parts = split(token, '.');
    if (parts.size() != 3)
      return nullopt;

    auto iv_bytes     = from_hex(parts[0]);
    auto cipher_bytes = from_hex(parts[1]);
    auto tag_bytes    = from_hex(parts[2]);

    if (!iv_bytes || !cipher_bytes || !tag_bytes)
      return nullopt;
    if (iv_bytes->size() != 12 || tag_bytes->size() != 16)
      return nullopt;

    auto key = load_key();

    try {
      aes_cpp::AES aes(aes_cpp::AESKeyLength::AES_256);
      vector<unsigned char> plain =
          aes.DecryptGCM(*cipher_bytes, key, *iv_bytes, /*aad=*/{}, *tag_bytes);

      string id_str(plain.begin(), plain.end());
      return static_cast<Id>(stoull(id_str));
    } catch (const exception &) {
      // Authentication failure or bad data
      return nullopt;
    }
  }

private:
  // ---- helpers ------------------------------------------------------------

  static vector<unsigned char> load_key() {
    string secret = SecretConfig::SECRET_KEY;
    vector<unsigned char> key(32, 0);
    size_t copy_len = min(secret.size(), size_t{32});
    for (size_t i = 0; i < copy_len; ++i)
      key[i] = static_cast<unsigned char>(secret[i]);
    return key;
  }

  static vector<unsigned char> random_iv() {
    vector<unsigned char> iv(12);
    random_device rd;
    for (auto &b : iv)
      b = static_cast<unsigned char>(rd());
    return iv;
  }

  static string to_hex(const vector<unsigned char> &bytes) {
    static constexpr char hex[] = "0123456789abcdef";
    string out;
    out.reserve(bytes.size() * 2);
    for (unsigned char b : bytes) {
      out += hex[b >> 4];
      out += hex[b & 0x0f];
    }
    return out;
  }

  static optional<vector<unsigned char>> from_hex(string_view s) {
    if (s.size() % 2 != 0)
      return nullopt;
    vector<unsigned char> out;
    out.reserve(s.size() / 2);
    for (size_t i = 0; i < s.size(); i += 2) {
      auto hi = hex_val(s[i]);
      auto lo = hex_val(s[i + 1]);
      if (!hi || !lo)
        return nullopt;
      out.push_back(static_cast<unsigned char>((*hi << 4) | *lo));
    }
    return out;
  }

  static optional<uint8_t> hex_val(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
    return nullopt;
  }

  static vector<string_view> split(string_view s, char delim) {
    vector<string_view> parts;
    size_t start = 0;
    for (size_t i = 0; i <= s.size(); ++i) {
      if (i == s.size() || s[i] == delim) {
        parts.push_back(s.substr(start, i - start));
        start = i + 1;
      }
    }
    return parts;
  }
};
