#pragma once

#include <string>

#include <stb_image.h>
#include <OpenGL/gl3.h>

class Texture {
public:
    GLuint id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;

    explicit Texture(const std::string& path, bool srgb = true) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) throw std::runtime_error("Failed to load texture: " + path);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        GLenum internal = srgb ? ((channels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8)
                               : ((channels == 4) ? GL_RGBA8 : GL_RGB8);
        
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // MacOS specific enum defs
        #ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
        #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
        #endif
        #ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
        #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
        #endif

        float maxAniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture() noexcept { if (id) glDeleteTextures(1, &id);}

    void bind(unsigned int unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }
};
