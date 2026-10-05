#pragma once

#include <memory>
#include "di/di.hpp"
#include "repositories/payment_repository.hpp"
#include "services/payment_service.hpp"
#include "controllers/payment_controller.hpp"
#include "routers/payment_router.hpp"

#include "utils/bearer_token.hpp"

class PaymentDI : public IDI {
private:
    std::unique_ptr<PaymentRepository> repo;
    std::unique_ptr<PaymentService> service;
    std::unique_ptr<PaymentController> controller;
    std::unique_ptr<PaymentRouter> router;

public:
    PaymentDI() {
        repo = std::make_unique<PaymentRepository>();
        auto auth_manager = std::make_unique<AuthManager>();
        service = std::make_unique<PaymentService>(std::move(repo), std::move(auth_manager));
        controller = std::make_unique<PaymentController>(std::move(service));
        router = std::make_unique<PaymentRouter>(*controller);
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
