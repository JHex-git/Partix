#pragma once

struct GLFWwindow;

namespace Partix
{
    class PartixEngine;
}

namespace PartixRenderer
{

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void Render(Partix::PartixEngine &partix_engine);

private:
    GLFWwindow *m_window;
};
} // namespace PartixRenderer
