#pragma once

#include <httplib.h>
#include <spdlog/spdlog.h>
#include <csignal>
#include "configs/app_config.hpp"
#include "di/user_di.hpp"

#include "middlewares/auth.hpp"
#include "middlewares/cors.hpp"
#include "middlewares/logger.hpp"
#include "middlewares/serializer.hpp"

class AppContainer {
private:
    httplib::Server server;
    std::vector<std::unique_ptr<IDI>> modules;

    // Static pointer to allow global signal handler to call server.stop()
    inline static httplib::Server* active_server = nullptr;

    static void signalHandler(int signum) {
        spdlog::warn("Received shutdown signal ({}). Gracefully stopping server...", signum);
        if (active_server) {
            active_server->stop();
        }
    }

    void setMiddleWares() {
        useLogger(server);
        useCors(server);
        useSerializer(server);
        useAuth(server);
    }

    void initializeModules() {
        modules.push_back(std::make_unique<UserDI>());
    }

    void setupRoutes() {
        for (auto &module : modules) {
            module->registerRoutes(server);
        }
    }

public:
    AppContainer() {
        active_server = &server;

        // Register system signals for graceful termination (Ctrl+C / Docker stop)
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        setMiddleWares();
        initializeModules();
        setupRoutes();
    }

    ~AppContainer() {
        active_server = nullptr;
    }

    void start(const std::string &host, int port) {
        spdlog::info("   Server starting...");
        spdlog::info("   Env : {}", AppConfig::APP_ENV);
        spdlog::info("   Host : {}", host);
        spdlog::info("   Port : {}", port);
        spdlog::info("   URL  : http://{}:{}", host, port);
    
        // server.listen blocks execution here until server.stop() is called
        if (!server.listen(host, port)) {
            spdlog::error("❌ Failed to start server or server stopped abruptly on {}:{}", host, port);
            return;
        }
    
        spdlog::info("Server stopped cleanly.");
    }
};