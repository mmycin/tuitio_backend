#pragma once

#include "controllers/controller.hpp"
#include "dtos/user_dto.hpp"
#include "services/user_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;

class UserController : public IController {
  private:
    std::unique_ptr<UserService> service;

  public:
    explicit UserController(std::unique_ptr<UserService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        try {
            string id_str = req.matches[1];
            int userId = std::stoi(id_str);

            auto user = service->getUserById(userId);
            auto user_dao = GetUserResponse::fromUser(user);

            sendJson(res, 200, user_dao.toJson());
        } catch (const exception &e) {
            sendError(res, 400, e.what());
        }
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
};
