#pragma once

#include <httplib.h>
#include "controllers/user_controller.hpp"

inline void registerUserRoutes(httplib::Server &server, UserController &controller) {
	server.Get(R"(/user/(\d+))", [&controller](const httplib::Request &req, httplib::Response &res) {
        controller.show(req, res);
    });
}