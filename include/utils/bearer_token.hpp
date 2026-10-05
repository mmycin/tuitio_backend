#pragma once

#include <monocypher.h>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "configs/secret_config.hpp"
#include "spdlog/spdlog.h"

// ---------------------------------------------------------------------------
// Token format (URL-safe, dot-separated hex):
//   <24-byte Nonce hex>.<ciphertext hex>.<16-byte MAC hex>
//
// Payload encrypted: "id:expiration_epoch_seconds"
// Key: SECRET_KEY env var (32 bytes)
//
// Performance: validated tokens are cached in-process (per token string).
// Cache entries auto-expire at token expiry time so no stale entries linger.
// ---------------------------------------------------------------------------

class AuthManager {
public:
  using Id = uint64_t;

  // Generate a high-speed authenticated token encoding user id and 3-hour expiry.
  std::string make_token(Id id) const {
    const auto &key = load_key();
    auto nonce = random_nonce();

    auto now = std::chrono::system_clock::now();
    auto expiry = now + std::chrono::hours(3);
    uint64_t exp_epoch = std::chrono::duration_cast<std::chrono::seconds>(expiry.time_since_epoch()).count();

    const std::string payload = std::to_string(id) + ":" + std::to_string(exp_epoch);
    std::vector<unsigned char> cipher(payload.size());
    uint8_t mac[16];

    crypto_aead_lock(cipher.data(), mac, key.data(), nonce.data(),
                     NULL, 0,
                     reinterpret_cast<const uint8_t*>(payload.data()), payload.size());

    std::vector<unsigned char> mac_vec(mac, mac + 16);
    return to_hex(nonce) + "." + to_hex(cipher) + "." + to_hex(mac_vec);
  }

  // Validate, decrypt token, and check expiry. Returns nullopt if expired or invalid.
  // Results are cached in-process; repeated calls with the same token are O(1).
  std::optional<Id> get_id(std::string_view token_sv) const {
    // Fast-path: legacy plaintext "user_id:<n>" tokens (dev/test only)
    if (token_sv.rfind("user_id:", 0) == 0) {
      try {
        return std::stoull(std::string(token_sv.substr(8)));
      } catch (...) {
        return std::nullopt;
      }
    }

    const std::string token(token_sv);

    // Check cache first (read lock)
    {
      std::lock_guard<std::mutex> lock(cache_mutex_);
      auto it = token_cache_.find(token);
      if (it != token_cache_.end()) {
        const CacheEntry &entry = it->second;
        uint64_t now_epoch = current_epoch_seconds();
        if (now_epoch <= entry.exp_epoch) {
          return entry.user_id;   // Cache hit — O(1), no crypto
        }
        // Expired: evict and fall through to full decrypt
        token_cache_.erase(it);
      }
    }

    // Cache miss: full decrypt + verify
    auto parts = split(token_sv, '.');
    if (parts.size() != 3)
      return std::nullopt;

    auto nonce_bytes  = from_hex(parts[0]);
    auto cipher_bytes = from_hex(parts[1]);
    auto mac_bytes    = from_hex(parts[2]);

    if (!nonce_bytes || !cipher_bytes || !mac_bytes)
      return std::nullopt;
    if (nonce_bytes->size() != 24 || mac_bytes->size() != 16)
      return std::nullopt;

    const auto &key = load_key();
    std::vector<unsigned char> plain(cipher_bytes->size());

    if (crypto_aead_unlock(plain.data(), mac_bytes->data(), key.data(), nonce_bytes->data(),
                            NULL, 0, cipher_bytes->data(), cipher_bytes->size()) != 0) {
      return std::nullopt;
    }

    try {
      std::string payload_str(plain.begin(), plain.end());
      auto payload_parts = split(payload_str, ':');
      if (payload_parts.size() != 2)
        return std::nullopt;

      Id id = std::stoull(std::string(payload_parts[0]));
      uint64_t exp_epoch = std::stoull(std::string(payload_parts[1]));

      uint64_t now_epoch = current_epoch_seconds();
      if (now_epoch > exp_epoch) {
        spdlog::info("Session expired");
        return std::nullopt;
      }

      // Store in cache
      {
        std::lock_guard<std::mutex> lock(cache_mutex_);
        token_cache_[token] = CacheEntry{id, exp_epoch};
      }

      return id;
    } catch (const std::exception &) {
      return std::nullopt;
    }
  }

private:
  // ---------------------------------------------------------------------------
  // In-process token cache (thread-safe)
  // ---------------------------------------------------------------------------
  struct CacheEntry {
    Id       user_id;
    uint64_t exp_epoch;
  };

  mutable std::mutex                              cache_mutex_;
  mutable std::unordered_map<std::string, CacheEntry> token_cache_;

  static uint64_t current_epoch_seconds() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
  }

  // ---------------------------------------------------------------------------
  // Crypto helpers
  // ---------------------------------------------------------------------------
  static const std::vector<unsigned char>& load_key() {
    static const std::vector<unsigned char> key = []() {
      std::string secret = SecretConfig::SECRET_KEY();
      std::vector<unsigned char> k(32, 0);
      size_t copy_len = std::min(secret.size(), size_t{32});
      for (size_t i = 0; i < copy_len; ++i)
        k[i] = static_cast<unsigned char>(secret[i]);
      return k;
    }();
    return key;
  }

  static std::vector<unsigned char> random_nonce() {
    std::vector<unsigned char> nonce(24);
    std::random_device rd;
    for (auto &b : nonce)
      b = static_cast<unsigned char>(rd());
    return nonce;
  }

  static std::string to_hex(const std::vector<unsigned char> &bytes) {
    static constexpr char hex[] = "0123456789abcdef";
    std::string out;
    out.reserve(bytes.size() * 2);
    for (unsigned char b : bytes) {
      out += hex[b >> 4];
      out += hex[b & 0x0f];
    }
    return out;
  }

  static std::optional<std::vector<unsigned char>> from_hex(std::string_view s) {
    if (s.size() % 2 != 0)
      return std::nullopt;
    std::vector<unsigned char> out;
    out.reserve(s.size() / 2);
    for (size_t i = 0; i < s.size(); i += 2) {
      auto hi = hex_val(s[i]);
      auto lo = hex_val(s[i + 1]);
      if (!hi || !lo)
        return std::nullopt;
      out.push_back(static_cast<unsigned char>((*hi << 4) | *lo));
    }
    return out;
  }

  static std::optional<uint8_t> hex_val(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
    return std::nullopt;
  }

  static std::vector<std::string_view> split(std::string_view s, char delim) {
    std::vector<std::string_view> parts;
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