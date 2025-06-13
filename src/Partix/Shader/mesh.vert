#include "shader.glsl"
SHADER_BEGIN
#include "view.glsl"
#include "particle_common.glsl"
#include "Math/quaternion.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location=0) in vec3 position;
layout(location=1) in vec2 tex_coords;

layout(location = 0) out uint outId;
layout(location = 1) out vec2 outTexCoords;

void main()
{
    uint id = gl_InstanceID;
    Particle particle = particle_buffer.particles[id];
    if (particle.alive) {
        gl_Position = view.projection_mat * view.view_mat * vec4(particle.position + Quaternion_Rotate(particle.rotation, position), 1);
        outTexCoords = tex_coords;
        outId = id;
    }
    else
    {
        gl_Position = vec4(-2, 0, 0, 1); // dead particles are not rendered
    }
}