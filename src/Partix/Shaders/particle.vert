#include "shader.glsl"
SHADER_BEGIN
// Declare the struct once
struct Particle {
    bool alive;
    bool prevAlive;
    float lifetime;
    vec3 position;
    vec3 velocity;
    float size;
    vec3 color;
    uvec4 seed;
};

layout(std140, binding = 0) uniform View
{
    mat4 view_mat;
    mat4 projection_mat;
    float prevTime;
    float currentTime;
    float deltaTime;
} view;

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

out uint ID;

void main()
{
    uint id = gl_VertexID;
    Particle particle = particle_buffer.particles[id];
    if (particle.alive) {
        ID = id;
        gl_Position = view.projection_mat * view.view_mat * vec4(particle.position, 1);
    }
    else
    {
        gl_Position = vec4(-2, 0, 0, 1); // dead particles are not rendered
    }
}