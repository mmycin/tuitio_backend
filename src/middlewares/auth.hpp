#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "configs/app_config.hpp"

inline void useAuth(httplib::Server &server) {
    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        if (req.path == "/auth/login" || req.path == "/auth/signup") {
        	return httplib::Server::HandlerResponse::Unhandled;
        }

        if (AppConfig::APP_ENV == "local") {
        	if(req.has_param("pass") && req.get_param_value("pass") == "true") {
         		return httplib::Server::HandlerResponse::Unhandled;
         	}
        }

        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
            res.status = 401;
            nlohmann::json error;
            error["status"] = 401;
            error["error"] = "Unauthorized: Missing or malformed token";
            res.set_content(error.dump(), "application/json");
            return httplib::Server::HandlerResponse::Handled; // Short-circuits request!
        }

        // Token is present; proceed to controllers/routers
        return httplib::Server::HandlerResponse::Unhandled;
    });
}