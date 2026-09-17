#pragma once

#include "command.hpp"
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include <cctype>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

class MakeResourcesCommand : public ICommand {
public:
    void register_command(CLI::App& app) override {
        auto* make_cmd = app.get_subcommand("make");
        if (!make_cmd) {
            make_cmd = app.add_subcommand("make", "Scaffold project components");
        }

        auto* resource_cmd = make_cmd->add_subcommand("resource", "Generate a full CRUD resource (repository, service, controller, router, DI)");
        resource_cmd->add_option("name", name_, "Name of the resource/entity (e.g., user, post, todo)")->required();

        resource_cmd->callback([this]() {
            this->execute();
        });
    }

private:
    std::string name_;

    std::string capitalize(const std::string& str) {
        if (str.empty()) return str;
        std::string result = str;
        result[0] = static_cast<char>(std::toupper(result[0]));
        return result;
    }

    std::string pluralize(const std::string& str) {
        if (str.empty()) return str;
        if (str.back() == 's') return str + "es";
        if (str.back() == 'y' && str.size() > 1) {
            char before = str[str.size() - 2];
            if (before != 'a' && before != 'e' && before != 'i' && before != 'o' && before != 'u') {
                return str.substr(0, str.size() - 1) + "ies";
            }
        }
        return str + "s";
    }

    std::string load_and_render_stub(const fs::path& stub_path, const std::unordered_map<std::string, std::string>& replacements) {
        std::ifstream file(stub_path);
        if (!file) {
            spdlog::error("Failed to open stub file: {}", stub_path.string());
            std::exit(1);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        for (const auto& [key, value] : replacements) {
            std::string placeholder = "{{" + key + "}}";
            size_t pos = 0;
            while ((pos = content.find(placeholder, pos)) != std::string::npos) {
                content.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        }

        return content;
    }

    void execute() {
        fs::path stubs_dir = "src/cli/stubs";
        fs::path repositories_dir = "src/repositories";
        fs::path services_dir = "src/services";
        fs::path controllers_dir = "src/controllers";
        fs::path routers_dir = "src/routers";
        fs::path di_dir = "src/di";

        fs::create_directories(repositories_dir);
        fs::create_directories(services_dir);
        fs::create_directories(controllers_dir);
        fs::create_directories(routers_dir);
        fs::create_directories(di_dir);

        std::string class_name = capitalize(name_);
        std::string plural_name = pluralize(name_);

        std::unordered_map<std::string, std::string> replacements = {
            {"name", name_},
            {"className", class_name},
            {"pluralName", plural_name}
        };

        fs::path repo_path = repositories_dir / (name_ + "_repository.hpp");
        std::string rendered_repo = load_and_render_stub(stubs_dir / "repository.hpp.stub", replacements);
        std::ofstream(repo_path) << rendered_repo;
        spdlog::info("Generated repository: {}", repo_path.string());

        fs::path service_path = services_dir / (name_ + "_service.hpp");
        std::string rendered_service = load_and_render_stub(stubs_dir / "service.hpp.stub", replacements);
        std::ofstream(service_path) << rendered_service;
        spdlog::info("Generated service:    {}", service_path.string());

        fs::path controller_path = controllers_dir / (name_ + "_controller.hpp");
        std::string rendered_controller = load_and_render_stub(stubs_dir / "controller.hpp.stub", replacements);
        std::ofstream(controller_path) << rendered_controller;
        spdlog::info("Generated controller: {}", controller_path.string());

        fs::path router_path = routers_dir / (name_ + "_router.hpp");
        std::string rendered_router = load_and_render_stub(stubs_dir / "router.hpp.stub", replacements);
        std::ofstream(router_path) << rendered_router;
        spdlog::info("Generated router:     {}", router_path.string());

        fs::path di_path = di_dir / (name_ + "_di.hpp");
        std::string rendered_di = load_and_render_stub(stubs_dir / "di.hpp.stub", replacements);
        std::ofstream(di_path) << rendered_di;
        spdlog::info("Generated DI:         {}", di_path.string());
    }
};
