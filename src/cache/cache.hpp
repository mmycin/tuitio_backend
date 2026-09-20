#pragma once

#include <optional>
#include <string>


#include "configs/redis_config.hpp"
#include <sw/redis++/redis++.h>


class Cache {
  public:
    struct Time {
        static constexpr int Second = 1;
        static constexpr int Minute = 60 * Second;
        static constexpr int Hour = 60 * Minute;
        static constexpr int Day = 24 * Hour;
    };

    static void set(const std::string &key, const std::string &value) {
        redis.set(key, value);
    }

    static void set(const std::string &key, const std::string &value, int ttl) {
        redis.setex(key, ttl, value);
    }

    static std::optional<std::string> get(const std::string &key) {
        return redis.get(key);
    }

    static void update(const std::string &key, const std::string &value) {
        redis.set(key, value);
    }

    static void update(const std::string &key, const std::string &value,
                       int ttl) {
        redis.setex(key, ttl, value);
    }

    static bool del(const std::string &key) {
        return redis.del(key) > 0;
    }

  private:
    inline static sw::redis::Redis redis{ "tcp://" + RedisConfig::REDIS_HOST +
                                          ":" + RedisConfig::REDIS_PORT };
};
