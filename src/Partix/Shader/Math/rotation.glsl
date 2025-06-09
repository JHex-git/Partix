#ifndef ROTATION_GLSL
#define ROTATION_GLSL

vec3 rotatePerpendicularAxis(vec3 v, vec3 axis, float angle) {
    // ensure rotation axis is perpendicular to v and is unit vector
    vec3 n = normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    
    return v * c + cross(n, v) * s;
}
#endif // ROTATION_GLSL