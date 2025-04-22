#ifndef RANDOM_GLSL
#define RANDOM_GLSL

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
#endif // RANDOM_GLSL