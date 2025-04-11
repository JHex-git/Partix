#include <glm/glm.hpp>
#include <Partix/EmitterSpawnComponent.h>

namespace Partix
{
struct alignas(16) EmitterSpawnData
{
    static std::string GetName() { return "EmitterSpawnData"; }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

void EmitterSpawnComponent::BuildData(DataBuildContext& context)
{
    auto& [data, buffer] = context.GetBuildData<EmitterSpawnData>();
    data.position = glm::vec3(0.0f, 0.0f, 0.0f);
    buffer.Transfer(&data, sizeof(EmitterSpawnData));
}
    
} // namespace Partix
