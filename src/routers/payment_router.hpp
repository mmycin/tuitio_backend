#pragma once

#include "routers/router.hpp"
#include "controllers/payment_controller.hpp"

class PaymentRouter : public IRouter {
private:
    PaymentController &controller;

public:
    explicit PaymentRouter(PaymentController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        registerResource(server, "/payments", controller);
    }
};
