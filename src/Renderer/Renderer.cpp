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
Renderer::Renderer(int width, int height) : m_width(width), m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, "Partix", nullptr, nullptr);
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

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    });
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &quad_vao);
    glDeleteBuffers(1, &quad_vbo);
    glDeleteRenderbuffers(1, &m_depth_stencil_renderbuffer);
    glDeleteFramebuffers(1, &m_hdr_fbo);
    glDeleteTextures(1, &m_hdr_texture);
    delete m_tonemap_program;
    glfwTerminate();
}

bool Renderer::Init()
{
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
    float quadVertices[] = {
        // position     texcoord
        -1.0f,  1.0f,   0.0f, 1.0f, // left top
        -1.0f, -1.0f,   0.0f, 0.0f, // left bottom
         1.0f, -1.0f,   1.0f, 0.0f, // right bottom
        
        -1.0f,  1.0f,   0.0f, 1.0f, // left top
         1.0f, -1.0f,   1.0f, 0.0f, // right bottom
         1.0f,  1.0f,   1.0f, 1.0f  // right top
    };
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texcoord
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Create Floating Point Framebuffer Object
    glGenFramebuffers(1, &m_hdr_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);
    glGenTextures(1, &m_hdr_texture);
    glBindTexture(GL_TEXTURE_2D, m_hdr_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdr_texture, 0);
    glGenRenderbuffers(1, &m_depth_stencil_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_stencil_renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to create framebuffer" << std::endl;
        return false;
    }

    Shader screen_vs(ShaderType::Vertex);
    if (!screen_vs.Load("screen.vert", {}))
    {
        std::cerr << "Failed to load screen vertex shader" << std::endl;
        return false;
    }
    Shader tonemap_ps(ShaderType::Fragment);
    if (!tonemap_ps.Load("tonemap.frag", {}))
    {
        std::cerr << "Failed to load tonemap fragment shader" << std::endl;
        return false;
    }
    m_tonemap_program = new ShaderProgram();
    std::vector<Shader> tonemap_shaders;
    tonemap_shaders.push_back(std::move(screen_vs));
    tonemap_shaders.push_back(std::move(tonemap_ps));
    return m_tonemap_program->Load(tonemap_shaders);
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
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(m_window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view.prevTime = view.currentTime;
        view.currentTime = static_cast<float>(glfwGetTime());
        view.deltaTime = view.currentTime - view.prevTime;

        partix_engine.Tick(view);

        // Postprocessing
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(quad_vao);
        m_tonemap_program->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_hdr_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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
