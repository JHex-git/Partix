#include "shader.glsl"
SHADER_BEGIN
#include "particle_common.glsl"
#include "view.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

flat in uint ID[];
layout(location = 0) out uint IDOut;
layout(location = 1) out vec2 TexCoord;

void main()
{
    Particle particle = particle_buffer.particles[ID[0]];

    if (particle.alive)
    {
        IDOut = ID[0];

        vec2 center = gl_in[0].gl_Position.xy;
        float spriteSize = particle.size;
        vec2 right = vec2(1, 0) * spriteSize * 0.5 * view.projection_mat[0][0];
        vec2 up = vec2(0, 1) * spriteSize * 0.5 * view.projection_mat[1][1];
        
        // left bottom
        vec4 pos = vec4(center - right - up, gl_in[0].gl_Position.zw);
        TexCoord = vec2(0, 0);
        gl_Position = pos;
        EmitVertex();
        // right bottom
        pos.xy = center + right - up;
        TexCoord = vec2(1, 0);
        gl_Position = pos;
        EmitVertex();
        // left top
        pos.xy = center - right + up;
        TexCoord = vec2(0, 1);
        gl_Position = pos;
        EmitVertex();
        // right top
        pos.xy = center + right + up;
        TexCoord = vec2(1, 1);
        gl_Position = pos;
        EmitVertex();
    }
    
    EndPrimitive();
}