#pragma once

#include <memory>
#include "di/di.hpp"
#include "repositories/user_repository.hpp"
#include "services/auth_service.hpp"
#include "controllers/auth_controller.hpp"
#include "routers/auth_router.hpp"
#include "utils/bearer_token.hpp"

class AuthDI : public IDI {
private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<AuthService> service;
    std::unique_ptr<AuthController> controller;
    std::unique_ptr<AuthRouter> router;
    std::unique_ptr<AuthManager> auth_manager;

public:
    AuthDI() {
        repo = std::make_unique<UserRepository>();
        auth_manager = std::make_unique<AuthManager>();
        service = std::make_unique<AuthService>(std::move(repo), std::move(auth_manager));
        controller = std::make_unique<AuthController>(std::move(service));
        router = std::make_unique<AuthRouter>(*controller);
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
