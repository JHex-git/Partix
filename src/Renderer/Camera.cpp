#include <renderer/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace PartixRenderer
{

Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, float fovy, float aspect, float z_near, float z_far) : 
    m_position(pos), 
    m_forward(glm::normalize(forward)),
    m_up(glm::normalize(glm::cross(glm::cross(forward, up), forward)))
{
    m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
    m_projection = glm::perspective(fovy, aspect, z_near, z_far);
    m_view_projection = m_projection * m_view;
}
} // namespace PartixRenderer
