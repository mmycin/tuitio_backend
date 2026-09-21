#pragma once

#include "controllers/user_controller.hpp"
#include "di/di.hpp"
#include "repositories/user_repository.hpp"
#include "routers/user_router.hpp"
#include "services/user_service.hpp"
#include <memory>
#include "bearer_token.hpp"


class UserDI : public IDI {
  private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<UserService> service;
    std::unique_ptr<UserController> controller;
    std::unique_ptr<UserRouter> router;
    std::unique_ptr<AuthManager> auth_manager;

  public:
    UserDI() {
        repo = std::make_unique<UserRepository>();
        auth_manager = std::make_unique<AuthManager>();
        service = std::make_unique<UserService>(std::move(repo),
                                                std::move(auth_manager));
        controller = std::make_unique<UserController>(std::move(service));
        router = std::make_unique<UserRouter>(*controller);
    }

    void registerRoutes(httplib::Server &server) override {
        router->registerRoutes(server);
    }

    IController &getController() override {
        return *controller;
    }

    IService &getService() override {
        return *service;
    }

    IRepository &getRepository() override {
        return *repo;
    }

    IRouter &getRouter() override {
        return *router;
    }
};
