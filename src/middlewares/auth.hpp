#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "configs/app_config.hpp"
#include "utils/bearer_token.hpp"

inline void useAuth(httplib::Server &server) {
    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        if (req.method == "OPTIONS") {
            return httplib::Server::HandlerResponse::Unhandled;
        }

        if (req.path == "/auth/login" || req.path == "/auth/signup" || req.path == "/health") {
            return httplib::Server::HandlerResponse::Unhandled;
        }

        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.rfind("Bearer ", 0) != 0 || auth_header.size() <= 7) {
            res.status = 401;
            nlohmann::json error;
            error["status"] = 401;
            error["error"] = "Unauthorized: Missing or malformed token";
            res.set_content(error.dump(), "application/json");
            return httplib::Server::HandlerResponse::Handled;
        }

        std::string token = auth_header.substr(7);
        AuthManager auth_mgr;
        if (!auth_mgr.get_id(token)) {
            res.status = 401;
            nlohmann::json error;
            error["status"] = 401;
            error["error"] = "Unauthorized: Invalid or expired token";
            res.set_content(error.dump(), "application/json");
            return httplib::Server::HandlerResponse::Handled;
        }

        return httplib::Server::HandlerResponse::Unhandled;
    });
}