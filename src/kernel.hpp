#pragma once

#include "di/user_di.hpp"
#include <httplib.h>
#include <iostream>

#include "di/user_di.hpp"
#include <httplib.h>
#include <iostream>

class AppContainer {
  private:
    httplib::Server server;

    std::vector<std::unique_ptr<IDI>> modules;

    void initializeModules() {
        modules.push_back(std::make_unique<UserDI>());
        // modules.push_back(std::make_unique<PostDI>());
    }

    void setupRoutes() {
        for (auto &module : modules) {
            module->registerRoutes(server);
        }
    }

  public:
    AppContainer() {
        initializeModules();
        setupRoutes();
    }

    void start(const std::string &host, int port) {
        std::cout << "Server started at port: " << port << std::endl;
        server.listen(host, port);
    }
};
