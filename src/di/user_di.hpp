#pragma once

#include <memory>
#include "controllers/user_controller.hpp"
#include "repositories/user_repository.hpp"
#include "services/user_service.hpp"

class UserDI {
	public:
		std::unique_ptr<UserController> controller;

	UserDI() {
		auto repo = std::make_unique<UserRepository>();
		auto service = std::make_unique<UserService>(std::move(repo));

		controller = std::make_unique<UserController>(std::move(service));
	}
};