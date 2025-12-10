//
// Created by Patrick Haas on 11/12/25.
//

#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

#include "engine/resources/Shader.hpp"
#include "engine/resources/Model.hpp"
#include "engine/resources/Texture.hpp"

template<class Resource, class Loader>
class ResourceManager {
protected:
    mutable std::unordered_map<std::string, std::weak_ptr<Resource>> resources_ = {};
    Loader loader;

public:
    explicit ResourceManager(Loader loader) : loader(std::move(loader)) {
    };

    std::shared_ptr<Resource> get(const std::string& name) const {
        auto it = resources_.find(name);
        if ((it == resources_.end()) or (it->second.expired())) {
            std::shared_ptr<Resource> new_res = loader.load(name);
            resources_[name] = new_res;
            return new_res;
        }
        return it->second.lock();
    }

    void clean() {
        for (auto it = resources_.begin(); it != resources_.end(); ) {
            if (it->second.expired()) {
                it = resources_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

struct ShaderLoader {
    const std::filesystem::path shader_dir;

    explicit ShaderLoader(const std::string& shader_dir) : shader_dir(shader_dir) {
    };

    std::shared_ptr<Shader> load(const std::string& shader_name) const {
        const auto vertex_shader_path = shader_dir / (shader_name + ".vert");
        const auto fragment_shader_path = shader_dir / (shader_name + ".frag");

        return std::make_shared<Shader>(
            vertex_shader_path.string(),
            fragment_shader_path.string()
        );
    }
};

struct TextureLoader {
    const std::filesystem::path texture_dir;

    explicit TextureLoader(const std::string& texture_dir) : texture_dir(texture_dir) {
    };

    std::shared_ptr<Texture> load(const std::string& texture_name, bool srgb = true) const {
        const auto texture_file_path = texture_dir / texture_name;

        return std::make_shared<Texture>(texture_file_path, srgb);
    }
};

struct ModelLoader {
    const std::filesystem::path model_dir;

    explicit ModelLoader(const std::string& model_dir) : model_dir(model_dir) {
    }

    std::shared_ptr<Model::Model> load(const std::string& model_name) const {
        const auto model_file_path = model_dir / model_name;

        return std::make_shared<Model::Model>(model_file_path);
    }
};

using ShaderManager = ResourceManager<Shader, ShaderLoader>;
using TextureManager = ResourceManager<Texture, TextureLoader>;
using ModelManager = ResourceManager<Model::Model, ModelLoader>;

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


    static TextureManager& texture_manager() {
        if (!initialized) {
            throw std::runtime_error("Managers not initialized! Call Managers::initialize() first.");
        }
        static TextureManager instance(TextureLoader(exe_dir_path / "textures"));
        return instance;
    }

    static ModelManager& model_manager() {
        if (!initialized) {
            throw std::runtime_error("Managers not initialized! Call Managers::initialize() first.");
        }
        static ModelManager instance(ModelLoader(exe_dir_path / "models"));
        return instance;
    }
};
