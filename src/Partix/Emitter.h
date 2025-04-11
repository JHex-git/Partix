#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Partix/Component.h>
#include <Partix/ShaderProgram.h>
namespace Partix
{

class Emitter
{
public:
    void AddComponent(std::unique_ptr<Component> component)
    {
        m_components.push_back(std::move(component));
    }

    void BuildData();
    
    bool Compile(const std::string shader_path);

    void Bind(unsigned int &bind_point);
private:
    std::vector<std::unique_ptr<Component>> m_components;
    ShaderProgram m_program;
    DataBuildContext m_context;
};
} // namespace Partix
