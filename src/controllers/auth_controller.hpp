#pragma once

#include "controllers/controller.hpp"
#include "dtos/auth_dto.hpp"
#include "dtos/user_dto.hpp"
#include "services/auth_service.hpp"
#include <exception>
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

    // Auth Controllers
    void login(const httplib::Request &req, httplib::Response &res) {
        try {
            auto loginReq = json::parse(req.body).get<LoginRequest>();
            loginReq.validate();

            auto [user, tokenOrMsg] =
                this->service->loginUser(loginReq.email, loginReq.password);

            if (user.id == 0) {
                sendError(res, 401, tokenOrMsg);
                return;
            }

            string token = tokenOrMsg;

            auto login_dao = LoginResponse(token, user);
            sendJson(res, 200, login_dao.toJson());

        } catch (const json::exception &e) {
            sendError(res, 400, "Invalid JSON format or missing fields");
        } catch (const std::exception &e) {
            sendError(res, 400, e.what());
        }
    }

    void signup(const httplib::Request &req, httplib::Response &res) {
        try {
            auto signupReq = json::parse(req.body).get<SignUpRequest>();
            signupReq.validate();
            auto user = this->service->signUp(signupReq.name, signupReq.email,
                                              signupReq.password);

            auto user_dao = GetUserResponse::fromUser(user);
            user_dao.validate();

            sendJson(res, 200, user_dao.toJson());

        } catch (const json::exception &e) {
            sendError(res, 400, "Invalid JSON format or missing fields");
        } catch (const std::exception &e) {
            sendError(res, 400, e.what());
        }
    }

    void verify(const httplib::Request &req, httplib::Response &res) {
        auto user = this->service->verifyUser(req);

        if (user.id == 0) {
            sendError(res, 404, "User not found");
        }
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 200, user_dto.toJson());
    }
};
