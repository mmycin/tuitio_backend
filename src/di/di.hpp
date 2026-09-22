#pragma once

#include <httplib.h>
#include "controllers/controller.hpp"
#include "services/service.hpp"
#include "repositories/repository.hpp"
#include "routers/router.hpp"

class IDI {
public:
    virtual ~IDI() = default;

    // 1. Mandatory route registration for the AppContainer
    virtual void registerRoutes(httplib::Server &server) = 0;

    // 2. Enforced architectural components that every module MUST expose
    virtual IController& getController() = 0;
    virtual IService& getService() = 0;
    virtual IRepository& getRepository() = 0;
    virtual IRouter& getRouter() = 0;
};