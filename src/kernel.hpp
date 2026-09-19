#pragma once

#define _HAS_STD_BYTE 0

#include <httplib.h>
#include <spdlog/spdlog.h>
#include <csignal>
#include "errors/base_error.hpp"
#include "errors/api.hpp"
#include "configs/app_config.hpp"
#include "di/user_di.hpp"
#include "di/auth_di.hpp"

#include "middlewares/auth.hpp"
#include "middlewares/cors.hpp"
#include "middlewares/logger.hpp"
#include "middlewares/serializer.hpp"

class AppContainer {
private:
    httplib::Server server;
    std::vector<std::unique_ptr<IDI>> modules;

    inline static httplib::Server* active_server = nullptr;

    static void signalHandler(int signum) {
        spdlog::warn("Received shutdown signal ({}). Gracefully stopping server...", signum);
        if (active_server) {
            active_server->stop();
        }
    }

    void setExceptionHandler() {
        server.set_exception_handler([](const httplib::Request &, httplib::Response &res,
                                        std::exception_ptr ep) {
            try {
                if (ep) {
                    std::rethrow_exception(ep);
                }
            } catch (const AppException &e) {
                res.status = e.getStatusCode();
                res.set_content(e.toJson().dump(), "application/json");
            } catch (const nlohmann::json::exception &e) {
                ApiError err("Invalid JSON format or missing fields: " + std::string(e.what()), 400);
                res.status = err.getStatusCode();
                res.set_content(err.toJson().dump(), "application/json");
            } catch (const std::invalid_argument &e) {
                ApiError err(e.what(), 400);
                res.status = err.getStatusCode();
                res.set_content(err.toJson().dump(), "application/json");
            } catch (const std::out_of_range &e) {
                ApiError err("Invalid parameter: " + std::string(e.what()), 400);
                res.status = err.getStatusCode();
                res.set_content(err.toJson().dump(), "application/json");
            } catch (const std::exception &e) {
                spdlog::error("Unhandled exception: {}", e.what());
                ApiError err("Internal Server Error", 500);
                res.status = err.getStatusCode();
                res.set_content(err.toJson().dump(), "application/json");
            } catch (...) {
                spdlog::error("Unknown exception occurred");
                ApiError err("Internal Server Error", 500);
                res.status = err.getStatusCode();
                res.set_content(err.toJson().dump(), "application/json");
            }
        });
    }

    void setMiddleWares() {
        useLogger(server);
        useCors(server);
        useSerializer(server);
        useAuth(server);
    }

    void initializeModules() {
        modules.push_back(std::make_unique<UserDI>());
        modules.push_back(std::make_unique<AuthDI>());
    }

    void setupRoutes() {
        for (auto &module : modules) {
            module->registerRoutes(server);
        }
    }

public:
    AppContainer() {
        active_server = &server;

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        setExceptionHandler();
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
    
        if (!server.listen(host, port)) {
            spdlog::error("❌ Failed to start server or server stopped abruptly on {}:{}", host, port);
            return;
        }
    
        spdlog::info("Server stopped cleanly.");
    }
};