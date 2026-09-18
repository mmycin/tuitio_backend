#pragma once

#include "controllers/auth_controller.hpp"
#include "httplib.h"
#include "routers/router.hpp"


class AuthRouter : public IRouter {
  private:
    AuthController &controller;

  public:
    explicit AuthRouter(AuthController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        server.Post("/auth/login", [this](const httplib::Request &req,
                                          httplib::Response &res) {
            controller.login(req, res);
        });

        server.Post("/auth/singup", [this](const httplib::Request &req,
                                           httplib::Response &res) {
            controller.signup(req, res);
        });
        
        server.Get("/auth/me", [this](const httplib::Request &req,
                                           httplib::Response &res) {
            controller.verify(req, res);
        });
    }
};
