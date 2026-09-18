#pragma once

#include "controllers/controller.hpp"
#include "dtos/auth_dto.hpp"
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

    // Auth Controllers
    void login(const httplib::Request &req, httplib::Response &res) {
        auto body = json::parse(req.body);

        string email = body["email"];
        string password = body["password"];

        auto [user, tokenOrMsg] = this->service->loginUser(email, password);
        if (user.id == 0) {
            sendError(res, 401, tokenOrMsg);
        }
        string token = tokenOrMsg;

        auto login_dao = LoginResponse(token, user);
        sendJson(res, 200, login_dao.toJson());
    }

    void signup(const httplib::Request &req, httplib::Response &res) {}
};
