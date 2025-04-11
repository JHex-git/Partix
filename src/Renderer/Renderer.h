#pragma once

struct GLFWwindow;
namespace PartixRenderer
{
    
class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void Render();

private:
    GLFWwindow *m_window;
};
} // namespace PartixRenderer
