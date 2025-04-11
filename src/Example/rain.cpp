#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/EmitterSpawnComponent.h>
int main()
{
    Partix::Emitter emitter;
    std::unique_ptr<Partix::EmitterSpawnComponent> spawn_component = std::make_unique<Partix::EmitterSpawnComponent>();
    emitter.AddComponent(std::move(spawn_component));
    if (emitter.Compile("rain.comp"))
    {
        emitter.BuildData();
        unsigned int bind_point = 0;
        emitter.Bind(bind_point);
        PartixRenderer::Renderer renderer(800, 600);
        renderer.Render();
    }

    return 0;
}