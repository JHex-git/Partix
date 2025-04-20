#include <vector>
#include <glad/glad.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>

int main()
{
    Partix::Emitter emitter;
    emitter.emitCountPerFrame = 1;
    emitter.position = glm::vec3(0, 5, -20);
    emitter.direction = glm::vec3(0, 1, 0);
    emitter.lifetime = 1000.0f;
    emitter.emitVelocity = 0.0f;
    emitter.spriteSize = 35.f;
    emitter.maxParticleCount = 1;

    PartixRenderer::Renderer renderer(800, 800);

    PartixEngine partix_engine;
    partix_engine.AddEmitter(emitter, "sprite.comp", "water.frag", {"noise.png"}, {0});

    renderer.Render(partix_engine);
    return 0;
}