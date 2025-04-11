#include <glad/glad.h>
#include <Renderer/Renderer.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace PartixRenderer
{

Renderer::Renderer(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(width, height, "Partix", nullptr, nullptr);
    if (m_window == nullptr)
    {
        glfwTerminate();
        throw "Failed to create GLFW window";
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    });
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw "Failed to initialize GLAD";
    }
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::Render()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}
} // namespace PartixRenderer
