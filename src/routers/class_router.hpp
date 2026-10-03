#pragma once

#include "routers/router.hpp"
#include "controllers/class_controller.hpp"

class ClassRouter : public IRouter {
private:
    ClassController &controller;

public:
    explicit ClassRouter(ClassController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        registerResource(server, "/classes", controller);
    }
};
