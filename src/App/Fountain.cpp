#include <vector>
#include <glad/glad.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>

struct Fountain
{
    float jitterAngleRange;
    float theta;
    alignas(16) glm::vec3 color;
};

int main()
{
    Partix::Emitter<Fountain> emitter;
    emitter.emitCountPerFrame = 1;
    emitter.position = glm::vec3(0, 0, -20);
    emitter.direction = glm::vec3(0, 1, 0);
    emitter.lifetime = 10.0f;
    emitter.emitVelocity = 10.0f;
    emitter.spriteSize = 0.15f;
    emitter.maxParticleCount = 10000;
    emitter.attributes.jitterAngleRange = 10.f;
    emitter.attributes.theta = 40.f;
    emitter.attributes.color = glm::vec3(1.0f, 0.f, 0.0f);

    PartixRenderer::Renderer renderer(800, 800);

    PartixEngine partix_engine;
    partix_engine.AddEmitter(emitter, "fountain.comp", "water.frag", {"noise.png"}, {0});

    emitter.emitVelocity = 15.0f;
    emitter.attributes.theta = 0.f;
    emitter.attributes.color = glm::vec3(0.0f, 1.f, 0.0f);
    partix_engine.AddEmitter(emitter, "fountain.comp", "water.frag", {"noise.png"}, {0});

    emitter.emitCountPerFrame = 3;
    emitter.emitVelocity = 20.0f;
    emitter.maxParticleCount = 20000;
    emitter.attributes.jitterAngleRange = 5.f;
    emitter.attributes.color = glm::vec3(1.0f, 1.f, 1.0f);
    partix_engine.AddEmitter(emitter, "fountain.comp", "water.frag", {"noise.png"}, {0});
    
    renderer.Render(partix_engine);
    return 0;
}