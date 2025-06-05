#ifndef UTILS_GLSL
#define UTILS_GLSL

bool CircleMask(vec2 uv, vec2 center, float radius)
{
    return length(uv - center) < radius;
}
#endif // UTILS_GLSL