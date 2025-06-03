#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer/Renderer.h>
#include <Renderer/Camera.h>
#include <Partix/PartixEngine.h>
#include <Partix/View.h>

using namespace Partix;
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw "Failed to initialize GLAD";
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    });
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::Render(Partix::PartixEngine &partix_engine)
{
    Camera camera(glm::vec3(0, 5, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::radians(60.0f), 1, 0.1f, 1000);

    View view;
    view.view_mat = camera.GetViewMatrix();
    view.projection_mat = camera.GetProjectionMatrix();
    view.prevTime = 0.0f;
    view.currentTime = 0.0f;
    view.deltaTime = 0.0f;
    
    glfwSwapInterval(0);
    unsigned int frame_count = 0;
    float averageFPS = 0;
    double prevTime = glfwGetTime();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view.prevTime = view.currentTime;
        view.currentTime = static_cast<float>(glfwGetTime());
        view.deltaTime = view.currentTime - view.prevTime;

        partix_engine.Tick(view);
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        frame_count++;
        if (frame_count == 1000)
        {
            double currentTime = glfwGetTime();
            float FPS = frame_count / static_cast<float>((currentTime - prevTime));
            if (averageFPS == 0)
            {
                averageFPS = FPS;
            }
            else
            {
                averageFPS = (averageFPS + FPS) / 2.0f;
            }
            frame_count = 0;
            prevTime = currentTime;
        }
    }
    std::cout << "FPS: " << averageFPS << std::endl;
}
} // namespace PartixRenderer
