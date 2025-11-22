//
// Created by Patrick Haas on 11/12/25.
//

#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "resources/Shader.hpp"
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"

using json = nlohmann::json;

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
    explicit ResourceManager(Loader loader) : loader(std::move(loader)) {
    };

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

    explicit ShaderLoader(const std::string& shader_dir) : shader_dir(shader_dir) {
    };

    Shader* load(const std::string& shader_name) const {
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

struct MeshLoader {
    const std::filesystem::path model_dir;

    explicit MeshLoader(const std::string& model_dir) : model_dir(model_dir) {
    };

    Mesh* load(const std::string& model_name) {
        const auto model_json_path = model_dir / (model_name + ".json");

        std::ifstream file(model_json_path);
        json json_data = json::parse(file);

        Mesh* new_mesh = new Mesh(json_data);
        return new_mesh;
    }

    void unload(Mesh* mesh) noexcept {
        delete mesh;
    }
};

struct TextureLoader {
    const std::filesystem::path texture_dir;

    explicit TextureLoader(const std::string& texture_dir) : texture_dir(texture_dir) {
    };

    Texture* load(const std::string& texture_name, bool srgb = true) {
        const auto texture_file_path = texture_dir / texture_name;

        Texture* new_tex = new Texture(texture_file_path, srgb);
        return new_tex;
    }

    void unload(Texture* texture) noexcept {
        delete texture;
    }
};

using ShaderManager = ResourceManager<Shader, ShaderLoader>;
using MeshManager = ResourceManager<Mesh, MeshLoader>;
using TextureManager = ResourceManager<Texture, TextureLoader>;

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

    static MeshManager& mesh_manager() {
        if (!initialized) {
            throw std::runtime_error("Managers not initialized! Call Managers::initialize() first.");
        }
        static MeshManager instance(MeshLoader(exe_dir_path / "models"));
        return instance;
    }

    static TextureManager& texture_manager() {
        if (!initialized) {
            throw std::runtime_error("Managers not initialized! Call Managers::initialize() first.");
        }
        static TextureManager instance(TextureLoader(exe_dir_path / "textures"));
        return instance;
    }
};
