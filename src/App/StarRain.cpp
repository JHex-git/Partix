#include <vector>
#include <glad/glad.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>
struct Rain
{
    alignas(8) glm::vec2 cover;
    float randomTheta;
};
int main()
{
    Partix::Emitter<Rain> emitter;
    emitter.emitCountPerFrame = 1;
    emitter.position = glm::vec3(0, 20, -20);
    emitter.direction = glm::vec3(0, -1, 0);
    emitter.lifetime = 10.0f;
    emitter.emitVelocity = 10.0f;
    emitter.spriteSize = 1.0f;
    emitter.maxParticleCount = 10000;
    emitter.attributes.cover = glm::vec2(40, 40);
    emitter.attributes.randomTheta = 30.0f;

    PartixRenderer::Renderer renderer(800, 800);

    PartixEngine partix_engine;
    partix_engine.AddEmitter(emitter, "rain.comp", "star.frag", {}, {});

    renderer.Render(partix_engine);
    return 0;
}