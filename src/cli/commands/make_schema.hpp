#pragma once

#include "command.hpp"
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <unordered_map>
#include <cctype>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

class MakeSchemaCommand : public ICommand {
public:
    void register_command(CLI::App& app) override {
        // 1. Create or get the parent "make" subcommand group
        auto* make_cmd = app.get_subcommand("make");
        if (!make_cmd) {
            make_cmd = app.add_subcommand("make", "Scaffold project components");
        }

        // 2. Add "schema" as a nested child subcommand under "make"
        auto* schema_cmd = make_cmd->add_subcommand("schema", "Generate a database HCL schema and optional model from stubs");

        schema_cmd->add_option("name", name_, "Name of the schema/entity (e.g., user, post)")->required();
        schema_cmd->add_flag("-m,--model", generate_model_, "Also generate a corresponding C++ model file");

        schema_cmd->callback([this]() {
            this->execute();
        });
    }

private:
    std::string name_;
    bool generate_model_ = false;

    int get_next_serial(const fs::path& schemas_dir) {
        int max_serial = 0;
        if (fs::exists(schemas_dir) && fs::is_directory(schemas_dir)) {
            for (const auto& entry : fs::directory_iterator(schemas_dir)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    if (filename.length() >= 5 && std::isdigit(filename[0]) && 
                        std::isdigit(filename[1]) && std::isdigit(filename[2]) && std::isdigit(filename[3])) {
                        int serial = std::stoi(filename.substr(0, 4));
                        if (serial > max_serial) max_serial = serial;
                    }
                }
            }
        }
        return max_serial + 1;
    }

    std::string capitalize(const std::string& str) {
        if (str.empty()) return str;
        std::string result = str;
        result[0] = static_cast<char>(std::toupper(result[0]));
        return result;
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
        fs::path schemas_dir = "src/database/schemas";
        fs::path models_dir = "src/models";

        fs::create_directories(schemas_dir);
        int serial = get_next_serial(schemas_dir);
        
        std::ostringstream serial_oss;
        serial_oss << std::setw(4) << std::setfill('0') << serial;
        
        std::string schema_filename = serial_oss.str() + "_" + name_ + ".hcl";
        fs::path schema_path = schemas_dir / schema_filename;

        fs::path schema_stub = stubs_dir / "schema.hcl.stub";
        std::string rendered_schema = load_and_render_stub(schema_stub, {{"name", name_}});

        std::ofstream(schema_path) << rendered_schema;
        spdlog::info("Generated schema: {}", schema_path.string());

        if (generate_model_) {
            fs::create_directories(models_dir);
            std::string class_name = capitalize(name_);
            fs::path model_path = models_dir / (name_ + "_model.hpp");

            fs::path model_stub = stubs_dir / "model.hpp.stub";
            std::string rendered_model = load_and_render_stub(model_stub, {{"className", class_name}});

            std::ofstream(model_path) << rendered_model;
            spdlog::info("Generated model:  {}", model_path.string());
        }
    }
};