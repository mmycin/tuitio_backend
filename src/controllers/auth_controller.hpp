#pragma once

#include "controllers/controller.hpp"
#include "dtos/auth_dto.hpp"
#include "dtos/user_dto.hpp"
#include "services/auth_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;
using json = nlohmann::json;

class AuthController : public IController {
  private:
    std::unique_ptr<AuthService> service;

  public:
    explicit AuthController(std::unique_ptr<AuthService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void login(const httplib::Request &req, httplib::Response &res) {
        auto loginReq = json::parse(req.body).get<LoginRequest>();
        auto [user, token] = this->service->loginUser(loginReq.email, loginReq.password);
        auto login_dto = LoginResponse(token, user);
        sendJson(res, 200, login_dto.toJson());
    }

    void signup(const httplib::Request &req, httplib::Response &res) {
        auto signupReq = json::parse(req.body).get<SignUpRequest>();
        auto user = this->service->signUp(signupReq.name, signupReq.email, signupReq.password);
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 201, user_dto.toJson());
    }

    void verify(const httplib::Request &req, httplib::Response &res) {
        auto user = this->service->verifyUser(req);
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 200, user_dto.toJson());
    }
};
