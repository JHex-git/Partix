#include <vector>
#include <glad/glad.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>

int main()
{
    Partix::Emitter emitter;
    emitter.emitCountPerFrame = 1;
    emitter.position = glm::vec3(0, 0, 0);
    emitter.direction = glm::vec3(0, 1, 0);
    emitter.lifetime = 10.0f;
    emitter.emitVelocity = 10.0f;
    emitter.jitterAngleRange = 10.f;

    PartixRenderer::Renderer renderer(800, 800);

    PartixEngine partix_engine;
    partix_engine.AddEmitter(emitter, 10000, "fountain.comp", "circle.frag", {"noise.png"}, {0});

    emitter.position = glm::vec3(2, 0, 0);
    partix_engine.AddEmitter(emitter, 10000, "fountain.comp", "circle.frag", {"noise.png"}, {0});

    renderer.Render(partix_engine);
    return 0;
}