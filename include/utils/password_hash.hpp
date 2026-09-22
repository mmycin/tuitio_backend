#pragma once

#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <monocypher.h>

namespace PasswordHash {
    // --- Private implementation details (hidden from outside files) ---
    namespace {
        constexpr size_t SALT_SIZE = 16;
        constexpr size_t HASH_SIZE = 32;
        constexpr uint32_t WORK_FACTOR = 3;
        constexpr uint32_t KB_MEMORY = 65536; // 64 MB of RAM

        inline std::string toHex(const uint8_t* data, size_t len) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (size_t i = 0; i < len; ++i) {
                ss << std::setw(2) << static_cast<int>(data[i]);
            }
            return ss.str();
        }

        inline std::vector<uint8_t> fromHex(const std::string& hex) {
            if (hex.size() % 2 != 0) throw std::invalid_argument("Invalid hex string size.");
            std::vector<uint8_t> bytes;
            bytes.reserve(hex.size() / 2);
            for (size_t i = 0; i < hex.size(); i += 2) {
                std::string byteString = hex.substr(i, 2);
                bytes.push_back(static_cast<uint8_t>(std::stoul(byteString, nullptr, 16)));
            }
            return bytes;
        }

        inline std::vector<uint8_t> generateSalt() {
            std::vector<uint8_t> salt(SALT_SIZE);
            std::random_device rd;
            std::generate(salt.begin(), salt.end(), [&rd]() { return static_cast<uint8_t>(rd()); });
            return salt;
        }
    } // namespace

    // --- Public API ---

    inline std::string hash(const std::string& password) {
        auto salt = generateSalt();
        uint8_t hash_bytes[HASH_SIZE];

        crypto_argon2_config config = {
            .algorithm = CRYPTO_ARGON2_I,
            .nb_blocks = KB_MEMORY,
            .nb_passes = WORK_FACTOR,
            .nb_lanes = 1
        };

        crypto_argon2_inputs inputs = {
            .pass = reinterpret_cast<const uint8_t*>(password.data()),
            .salt = salt.data(),
            .pass_size = static_cast<uint32_t>(password.size()),
            .salt_size = static_cast<uint32_t>(salt.size())
        };

        crypto_argon2_extras extras = {0};
        std::vector<uint8_t> work_area((size_t)config.nb_blocks * 1024);

        crypto_argon2(
            hash_bytes, HASH_SIZE,
            work_area.data(),
            config,
            inputs,
            extras
        );

        return toHex(salt.data(), salt.size()) + ":" + toHex(hash_bytes, HASH_SIZE);
    }

    inline bool verify(const std::string& password, const std::string& stored_record) {
        auto delimiter_pos = stored_record.find(':');
        if (delimiter_pos == std::string::npos) return false;

        try {
            auto salt_hex = stored_record.substr(0, delimiter_pos);
            auto hash_hex = stored_record.substr(delimiter_pos + 1);

            auto salt = fromHex(salt_hex);
            auto expected_hash = fromHex(hash_hex);

            if (expected_hash.size() != HASH_SIZE) return false;

            uint8_t computed_hash[HASH_SIZE];
            crypto_argon2_config config = {
                .algorithm = CRYPTO_ARGON2_I,
                .nb_blocks = KB_MEMORY,
                .nb_passes = WORK_FACTOR,
                .nb_lanes = 1
            };

            crypto_argon2_inputs inputs = {
                .pass = reinterpret_cast<const uint8_t*>(password.data()),
                .salt = salt.data(),
                .pass_size = static_cast<uint32_t>(password.size()),
                .salt_size = static_cast<uint32_t>(salt.size())
            };

            crypto_argon2_extras extras = {0};
            std::vector<uint8_t> work_area((size_t)config.nb_blocks * 1024);

            crypto_argon2(
                computed_hash, HASH_SIZE,
                work_area.data(),
                config,
                inputs,
                extras
            );

            return crypto_verify32(computed_hash, expected_hash.data()) == 0;
        } catch (...) {
            return false;
        }
    }
}