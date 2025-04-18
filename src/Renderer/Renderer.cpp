#include <glad/glad.h>
#include <Renderer/Renderer.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <Partix/Particle.h>
#include <Partix/Emitter.h>
#include <Partix/Shader.h>
#include <Partix/ShaderProgram.h>
#include <Partix/Frame.h>
#include <Renderer/Camera.h>
#include <Partix/Texture.h>

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

struct FView {
    glm::mat4 view_mat;
    glm::mat4 projection_mat;
};

void Renderer::Render()
{
    // 现代OpenGL（核心模式）要求必须绑定一个VAO，即使未显式使用。
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Camera camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::radians(60.0f), 1, 0.1, 1000);

    GLuint uboFView;
    glGenBuffers(1, &uboFView);
    glBindBuffer(GL_UNIFORM_BUFFER, uboFView);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(FView), nullptr, GL_DYNAMIC_DRAW); // 分配内存
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboFView); // 绑定到 binding = 0
    FView view;
    view.view_mat = camera.GetViewMatrix();
    view.projection_mat = camera.GetProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, uboFView);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FView), &view);

    GLuint ssbo[2];
    GLuint binding_points[2] = {1, 2};
    std::vector<Particle> particles(100000);
    glGenBuffers(2, ssbo);
    static_assert(sizeof(Particle) == 64);
    for (int i = 0; i < 2; ++i)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_points[i], ssbo[i]);
    }

    GLuint zero = 0;
    GLuint atomic_buffer;
    glGenBuffers(1, &atomic_buffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &zero, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_buffer);

    GLuint emitter_ubo, frame_ubo;

    glGenBuffers(1, &emitter_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, emitter_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Emitter), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, emitter_ubo);

    glGenBuffers(1, &frame_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, frame_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Frame), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, frame_ubo);

    Shader simulate_shader(ShaderType::Compute);
    simulate_shader.Load("particle.comp");
    ShaderProgram simulate_program;
    simulate_program.Load({ simulate_shader });

    Shader display_vert(ShaderType::Vertex);
    Shader display_frag(ShaderType::Fragment);
    display_vert.Load("particle.vert");
    display_frag.Load("particle.frag");
    ShaderProgram display_program;
    display_program.Load({display_vert, display_frag});

    Partix::Texture noise;
    noise.Load("noise.png");
    noise.Bind(0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int total_particles = particles.size();
    int local_size_x = 256; // Match the local_size_x in your compute shader
    int num_work_groups = (total_particles + local_size_x - 1) / local_size_x;
    glfwSwapInterval(0); // 禁用 VSync
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置清屏颜色（黑色）

    Emitter emitter;
    emitter.emitCountPerFrame = 1;
    emitter.position = glm::vec3(0, 0, 0);
    emitter.direction = glm::vec3(0, 1, 0);
    emitter.lifetime = 10.0f;
    emitter.emitVelocity = 10.0f;
    emitter.jitterAngleRange = 10.f;
    glBindBuffer(GL_UNIFORM_BUFFER, emitter_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Emitter), &emitter);

    Frame frame;
    frame.prevTime = 0.0f;
    frame.currentTime = 0.0f;
    frame.deltaTime = 0.0f;

    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲区

        glBindBuffer(GL_UNIFORM_BUFFER, frame_ubo);
        frame.prevTime = frame.currentTime;
        frame.currentTime = static_cast<float>(glfwGetTime());
        frame.deltaTime = frame.currentTime - frame.prevTime;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Frame), &frame);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

        simulate_program.Bind();
        glDispatchCompute(num_work_groups, 1, 1);

        display_program.Bind();
        glDrawArrays(GL_POINTS, 0, particles.size());

        std::swap(binding_points[0], binding_points[1]);
        for (int i = 0; i < 2; ++i)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_points[i], ssbo[i]);
        }
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
}
} // namespace PartixRenderer
