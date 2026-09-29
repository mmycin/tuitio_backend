#pragma once

#include "controllers/controller.hpp"
#include "dtos/user_dto.hpp"
#include "errors/api.hpp"
#include "errors/validation.hpp"
#include "services/user_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;
using json = nlohmann::json;

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
        int userId = getIdFromRequest(req);

        auto user = service->getUserById(userId);
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 200, user_dto.toJson());
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        int userId = getIdFromRequest(req);

        string token = this->getTokenFromHeader(req);

        UpdateUserRequest updateReq = json::parse(req.body);        
        User user = this->service->updateUser(token, userId, updateReq);

        if(user.id == 0) {
            sendError(res, 501, "Can not update user");
        } else {
            sendJson(res, 200, UpdateUserResponse(true, user).toJson());
        }
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        int userId = getIdFromRequest(req);

        string token = this->getTokenFromHeader(req);

        bool isDeleted = this->service->deleteUserById(token, userId);

        if (isDeleted) {
            sendJson(res, 200, DeleteUserResponse(isDeleted).toJson());
        } else {
            sendError(res, 401, DeleteUserResponse(isDeleted).toJson());
        }
    }
};
