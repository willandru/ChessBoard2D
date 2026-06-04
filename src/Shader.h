#pragma once

#include <filesystem>

class Shader
{
public:
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool isValid() const;
    void use() const;
    void setColor(float r, float g, float b) const;

private:
    unsigned int id_ = 0;
};
