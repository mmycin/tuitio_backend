// #pragma once

// #include <optional>
// #include <string>

// #include <nlohmann/json.hpp>
// #include <sw/redis++/redis++.h>

// #include "configs/redis_config.hpp"

// using json = nlohmann::json;

// class Cache {
// public:
//     struct Time {
//         static constexpr int Second = 1;
//         static constexpr int Minute = 60 * Second;
//         static constexpr int Hour   = 60 * Minute;
//         static constexpr int Day    = 24 * Hour;
//     };

//     static void set(const std::string& key, const std::string& value) {
//         redis().set(key, value);
//     }

//     static void set(const std::string& key,
//                     const std::string& value,
//                     int ttl) {
//         redis().setex(key, ttl, value);
//     }

//     static void set(const std::string& key, const json& value) {
//         set(key, value.dump());
//     }

//     static void set(const std::string& key,
//                     const json& value,
//                     int ttl) {
//         set(key, value.dump(), ttl);
//     }

//     static std::optional<std::string> get(const std::string& key) {
//         return redis().get(key);
//     }

//     static std::optional<json> get_json(const std::string& key) {
//         auto value = get(key);

//         if (!value) {
//             return std::nullopt;
//         }

//         return json::parse(*value);
//     }

//     static bool del(const std::string& key) {
//         return redis().del(key) > 0;
//     }

// private:
//     static sw::redis::Redis& redis() {
//         static sw::redis::Redis instance{
//             "tcp://" + RedisConfig::REDIS_HOST() +
//             ":" + RedisConfig::REDIS_PORT()
//         };

//         return instance;
//     }
// };
