#version 450
// Declare the struct once
struct Particle {
    bool alive;
    bool prevAlive;
    float lifetime;
    vec3 position;
    vec3 velocity;
    float size;
    uvec4 seed;
};

layout(binding = 0) uniform FView
{
    mat4 view_mat;
    mat4 projection_mat;
} View;

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
        gl_Position = View.projection_mat * View.view_mat * vec4(particle.position, 1);
    }
    else
    {
        gl_Position = vec4(-2, 0, 0, 1); // dead particles are not rendered
    }
}