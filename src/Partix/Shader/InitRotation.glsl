#ifndef INITROTATION_GLSL
#define INITROTATION_GLSL
#include "random.glsl"

InitRotation_Init(inout Particle particle)
{
    particle.rotation = RandomPointFromSphere(particle);
}