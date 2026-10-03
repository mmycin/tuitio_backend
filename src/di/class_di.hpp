#pragma once

#include <memory>
#include "di/di.hpp"
#include "repositories/class_repository.hpp"
#include "services/class_service.hpp"
#include "controllers/class_controller.hpp"
#include "routers/class_router.hpp"

class ClassDI : public IDI {
private:
    std::unique_ptr<ClassRepository> repo;
    std::unique_ptr<ClassService> service;
    std::unique_ptr<ClassController> controller;
    std::unique_ptr<ClassRouter> router;

public:
    ClassDI() {
        repo = std::make_unique<ClassRepository>();
        service = std::make_unique<ClassService>(std::move(repo));
        controller = std::make_unique<ClassController>(std::move(service));
        router = std::make_unique<ClassRouter>(*controller);
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
