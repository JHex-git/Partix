#ifndef QUATERNION_GLSL
#define QUATERNION_GLSL
#include "random.glsl"

#define Quaternion vec4 // imaginary in x, y, z, real in w

Quaternion Quaternion_Multiply(Quaternion a, Quaternion b)
{
    return Quaternion(
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    );
}

Quaternion Quaternion_Conjugate(Quaternion q)
{
    return Quaternion(-q.xyz, q.w);
}

float Quaternion_Length(Quaternion q)
{
    return length(q);
}

Quaternion Quaternion_Normalize(Quaternion q)
{
    float len = Quaternion_Length(q);
    if (len == 0.0) {
        return Quaternion(0.0, 0.0, 0.0, 1.0); // Return identity quaternion if length is zero
    }
    return Quaternion(q.x / len, q.y / len, q.z / len, q.w / len);
}

vec3 Quaternion_Rotate(Quaternion q, vec3 point)
{
    Quaternion qv = Quaternion(point.x, point.y, point.z, 0.0);
    Quaternion qr = Quaternion_Multiply(Quaternion_Multiply(q, qv), Quaternion_Conjugate(q));
    return vec3(qr.x, qr.y, qr.z);
}

vec3 Quaternion_Ratate(Quaternion q, vec3 point, vec3 center)
{
    // Translate the point to the origin
    vec3 translated = point - center;
    // Rotate the translated point
    vec3 rotated = Quaternion_Rotate(q, translated);
    // Translate back to the original position
    return rotated + center;
}

vec3 Quaternion_RatateAroundAxis(Quaternion q, vec3 point, vec3 axis, float angle)
{
    // Normalize the axis
    vec3 n = normalize(axis);
    // Create a quaternion representing the rotation
    Quaternion rotation = Quaternion(cos(angle * 0.5), n.x * sin(angle * 0.5), n.y * sin(angle * 0.5), n.z * sin(angle * 0.5));
    // Rotate the vector using the quaternion
    return Quaternion_Rotate(rotation, point);
}

vec3 Quaternion_RatateAroundAxis(Quaternion q, vec3 point, vec3 axis, float angle, vec3 axis_point)
{
    vec3 translated = point - axis_point;
    vec3 rotated = Quaternion_RatateAroundAxis(q, translated, axis, angle);
    return rotated + axis_point;
}

// Generate a random quaternion
Quaternion Quaternion_RandomQuaternion(inout Particle particle)
{
    Quaternion quat = RandomFloat4NormalDistribution(particle);
    return Quaternion_Normalize(quat);
}

#endif