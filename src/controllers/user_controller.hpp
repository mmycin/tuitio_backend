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
        string id_str = req.matches[1];
        Validator::assertField(!id_str.empty(), "id", "User id is required");

        int userId = 0;
        try {
            userId = std::stoi(id_str);
        } catch (const std::exception &) {
            json details;
            details["id"] = "User id must be a valid integer";
            throw ValidationError(details, "Validation failed");
        }

        auto user = service->getUserById(userId);
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 200, user_dto.toJson());
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        string id_str = req.matches[1];
        Validator::assertField(!id_str.empty(), "id", "User id is required");

        int userId = 0;
        try {
            userId = std::stoi(id_str);
        } catch (const std::exception &) {
            json details;
            details["id"] = "User id must be a valid integer";
            throw ValidationError(details, "Validation failed");
        }

        string token = this->getTokenFromHeader(req);

        UpdateUserRequest updateReq = json::parse(req.body);

        
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        string id_str = req.matches[1];
        Validator::assertField(!id_str.empty(), "id", "User id is required");

        int userId = 0;
        try {
            userId = std::stoi(id_str);
        } catch (const std::exception &) {
            json details;
            details["id"] = "User id must be a valid integer";
            throw ValidationError(details, "Validation failed");
        }

        string token = this->getTokenFromHeader(req);

        bool isDeleted = this->service->deleteUserById(token, userId);

        if (isDeleted) {
            sendJson(res, 200, DeleteUserResponse(isDeleted).toJson());
        } else {
            sendError(res, 401, DeleteUserResponse(isDeleted).toJson());
        }
    }
};
