#pragma once

#include <memory>
#include "di/di.hpp"
#include "repositories/cycle_repository.hpp"
#include "services/cycle_service.hpp"
#include "controllers/cycle_controller.hpp"
#include "routers/cycle_router.hpp"

class CycleDI : public IDI {
private:
    std::unique_ptr<CycleRepository> repo;
    std::unique_ptr<CycleService> service;
    std::unique_ptr<CycleController> controller;
    std::unique_ptr<CycleRouter> router;
    std::unique_ptr<AuthManager> auth_manager;


public:
    CycleDI() {
        repo = std::make_unique<CycleRepository>();
        service = std::make_unique<CycleService>(std::move(repo),
                                                std::move(auth_manager));
        controller = std::make_unique<CycleController>(std::move(service));
        router = std::make_unique<CycleRouter>(*controller);
    }

    void registerRoutes(httplib::Server &server) override {
        router->registerRoutes(server);
    }

    IController& getController() override {
        return *controller;
    }

    IService& getService() override {
        return *service;
    }

    IRepository& getRepository() override {
        return *repo;
    }

    IRouter& getRouter() override {
        return *router;
    }
};
