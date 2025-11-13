//
// Created by Patrick Haas on 11/12/25.
//

#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include "Shader.hpp"

template<class Resource, class Loader>
class ResourceManager {
private:
    struct Entry {
        Resource* ptr;
        int ref_count;
    };
    std::unordered_map<std::string, Entry> resources = {};
    Loader loader;
public:
    explicit ResourceManager(Loader loader) : loader(std::move(loader)) {};

    Resource* get(const std::string& name) {
        auto it = resources.find(name);
        if (it == resources.end()) {
            Resource* new_res = loader.load(name);
            Entry new_entry{new_res, 1};
            resources[name] = new_entry;
            return new_res;
        }
        it->second.ref_count += 1;
        return it->second.ptr;
    }

    void release(const std::string& name) noexcept {
        auto it = resources.find(name);
        if (it == resources.end()) return;

        it->second.ref_count -= 1;
        if (it->second.ref_count < 1) {
            loader.unload(it->second.ptr);
            resources.erase(it);
        }
    }
};

struct ShaderLoader {
    const std::filesystem::path shader_dir;

    explicit ShaderLoader(const std::string& shader_dir) : shader_dir(shader_dir) {};

    Shader* load(const std::string& shader_name) {
        const auto vertex_shader_path = shader_dir / (shader_name + ".vert");
        const auto fragment_shader_path = shader_dir / (shader_name + ".frag");

        Shader* shader = new Shader(
            vertex_shader_path.string(),
            fragment_shader_path.string()
            );
        return shader;
    }

    void unload(Shader* shader) noexcept {
        delete shader;
    }
};

using ShaderManager = ResourceManager<Shader, ShaderLoader>;

class Managers {
private:
    static std::filesystem::path exe_dir_path;
    static bool initialized;
public:
    static void initialize(const std::filesystem::path& exe_path) {
        Managers::exe_dir_path = exe_path;
        Managers::initialized = true;
    }

    static ShaderManager& shader_manager() {
        if (!initialized) {
            throw std::runtime_error("Managers not initialized! Call Managers::initialize() first.");
        }
        static ShaderManager instance(ShaderLoader(exe_dir_path / "shaders"));
        return instance;
    }
};