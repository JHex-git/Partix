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

    EmitterShaderInfo emitter_shader_info;
    emitter_shader_info.simulate_shader_path = "rain.comp";
    emitter_shader_info.sprite_shader_path = "star.frag";
    emitter_shader_info.sprite_shader_texture_paths = {};
    emitter_shader_info.sprite_texture_bindings = {};
    emitter_shader_info.defines[EmitterShaderInfo::ParticleExtraAttributes] = "vec2 cover;float randomTheta;";

    PartixRenderer::Renderer renderer(800, 800);

    PartixEngine partix_engine;
    partix_engine.AddEmitter(emitter, emitter_shader_info);

    renderer.Render(partix_engine);
    return 0;
}