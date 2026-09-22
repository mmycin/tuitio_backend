#pragma once

#include "routers/router.hpp"
#include "controllers/user_controller.hpp"

class UserRouter : public IRouter {
private:
    UserController &controller;

public:
    explicit UserRouter(UserController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        registerResource(server, "/users", controller);
    }
};