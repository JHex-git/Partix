#ifndef RANDOM_GLSL
#define RANDOM_GLSL
#include "particle_common.glsl"

#define PI 3.14159265358979323846
/*
    This random number generator is copied from UE5
*/

// 4D random number generator inspired by PCGs (permuted congruential generator)
// Using a **simple** Feistel cipher in place of the usual xor shift permutation step
// http://jcgt.org/published/0009/03/02/
// @param v = 4D integer coordinate
// @return four elements w/ 32 random bits each (0-0xffffffff).
uvec4 Rand4DPCG32(uvec4 p)
{
	// taking a signed int then reinterpreting as unsigned gives good behavior for negatives
	uvec4 v = uvec4(p);

	// Linear congruential step.
	v = v * 1664525u + 1013904223u;

	// shuffle
	v.x += v.y*v.w;
	v.y += v.z*v.x;
	v.z += v.x*v.y;
	v.w += v.y*v.z;

	// xoring high bits into low makes all 32 bits pretty good
	v ^= (v >> 16u);

	// final shuffle
	v.x += v.y*v.w;
	v.y += v.z*v.x;
	v.z += v.x*v.y;
	v.w += v.y*v.z;

	return v;
}

/*
    This random number generator is copied from UE5
*/
uvec4 RandomUInt4(inout Particle particle)
{
	++particle.seed.x;
	particle.seed.w = particle.seed.x ^ particle.seed.y;
	return Rand4DPCG32(particle.seed);
}
uint   RandomUInt(inout Particle particle) { return RandomUInt4(particle).x; }
uvec2  RandomUInt2(inout Particle particle) { return RandomUInt4(particle).xy; }
uvec3  RandomUInt3(inout Particle particle) { return RandomUInt4(particle).xyz; }

vec4 RandomFloat4(inout Particle particle)
{
	uvec4 v = RandomUInt4(particle);
	return vec4((v >> 8) & 0x00ffffff) / 16777216.0; // 0x01000000 == 16777216
}
float  RandomFloat(inout Particle particle)  { return RandomFloat4(particle).x; }
vec2 RandomFloat2(inout Particle particle) { return RandomFloat4(particle).xy; }
vec3 RandomFloat3(inout Particle particle) { return RandomFloat4(particle).xyz; }

vec3 RandomPointFromSphere(inout Particle particle)
{
	vec2 uv = RandomFloat2(particle);
	float phi = uv.x * 2.0 * PI;
	float theta = acos(2.0 * uv.y - 1.0);
	return vec3(
		sin(theta) * cos(phi),
		sin(theta) * sin(phi),
		cos(theta)
	);
}
#endif // RANDOM_GLSL