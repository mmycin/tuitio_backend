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

    // Read the authenticated user id injected by the auth middleware.
    // The middleware already validated the token and stored the id in X-User-Id.
    uint64_t getAuthUserId(const httplib::Request &req) {
        std::string hdr = req.get_header_value("X-User-Id");
        if (hdr.empty()) {
            throw UnauthorizedError("Missing authentication context");
        }
        try {
            return std::stoull(hdr);
        } catch (...) {
            throw UnauthorizedError("Invalid authentication context");
        }
    }

  public:
    explicit UserController(std::unique_ptr<UserService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        int userId = getIdFromRequest(req);
        uint64_t authId = getAuthUserId(req);

        if (authId != static_cast<uint64_t>(userId)) {
            throw UnauthorizedError("You are not authorized to view this user profile");
        }

        auto user = service->getUserById(userId);
        auto user_dto = GetUserResponse::fromUser(user);
        sendJson(res, 200, user_dto.toJson());
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        int userId = getIdFromRequest(req);
        uint64_t authId = getAuthUserId(req);

        UpdateUserRequest updateReq = json::parse(req.body);
        User user = this->service->updateUser(authId, userId, updateReq);

        if (user.id == 0) {
            sendError(res, 501, "Can not update user");
        } else {
            sendJson(res, 200, UpdateUserResponse(true, user).toJson());
        }
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        int userId = getIdFromRequest(req);
        uint64_t authId = getAuthUserId(req);

        bool isDeleted = this->service->deleteUserById(authId, userId);

        if (isDeleted) {
            sendJson(res, 200, DeleteUserResponse(isDeleted).toJson());
        } else {
            sendError(res, 401, DeleteUserResponse(isDeleted).toJson());
        }
    }
};
