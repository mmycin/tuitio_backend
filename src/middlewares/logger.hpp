#pragma once

#include <chrono>
#include <httplib.h>
#include <spdlog/spdlog.h>

inline void useLogger(httplib::Server &server) {
    using Clock = std::chrono::steady_clock;

    thread_local Clock::time_point start;

    server.set_pre_routing_handler(
        [&](const httplib::Request &req, httplib::Response &res) {
            start = Clock::now();
            return httplib::Server::HandlerResponse::Unhandled;
        }
    );

    server.set_logger(
        [&](const httplib::Request &req, const httplib::Response &res) {
            const auto duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    Clock::now() - start
                );

            spdlog::info(
                "{} {} -> {} ({} ms)",
                req.method,
                req.path,
                res.status,
                duration.count()
            );
        }
    );
}
