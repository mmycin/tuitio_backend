#pragma once

#include "routers/router.hpp"
#include "controllers/cycle_controller.hpp"

class CycleRouter : public IRouter {
private:
    CycleController &controller;

public:
    explicit CycleRouter(CycleController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        registerResource(server, "/cycles", controller);
    }
};
