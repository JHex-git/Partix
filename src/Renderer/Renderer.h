#pragma once
#include <memory>
#include <glad/glad.h>
#include <Partix/ShaderProgram.h>

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

    bool Init();

    void Render(Partix::PartixEngine &partix_engine);

private:
    GLFWwindow *m_window;
    GLuint quad_vao;
    GLuint quad_vbo;
    GLuint m_hdr_fbo;
    GLuint m_hdr_texture;
    GLuint m_depth_stencil_renderbuffer;
    Partix::ShaderProgram* m_tonemap_program;
    int m_width;
    int m_height;
};
} // namespace PartixRenderer
