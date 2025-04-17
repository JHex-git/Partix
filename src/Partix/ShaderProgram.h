#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Partix/Shader.h>

namespace Partix
{
    
class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    bool Load(const std::vector<Shader> &shaders) const;
    inline unsigned int GetID() const { return m_id; }

    void SetParam(const std::string &param, int value) const;
    void SetParam(const std::string &param, const glm::mat4 &value) const;

    void Bind() const;
    
private:
    unsigned int m_id;
};
} // namespace Partix
