#pragma once
#include <glm/glm.hpp>

namespace PartixRenderer
{

class Camera
{
public:
    Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, float fovy, float aspect, float z_near, float z_far);

    inline const glm::mat4& GetViewMatrix() const { return m_view; }
    inline const glm::mat4& GetProjectionMatrix() const { return m_projection; }
    inline const glm::mat4& GetViewProjectionMatrix() const { return m_view_projection; }

private:
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;

    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_view_projection;
};
} // namespace PartixRenderer
