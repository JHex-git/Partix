#include "shader.glsl"
SHADER_BEGIN

#define PARTIX_MODULES \
	PARTIX_MODULE(EmitterSpawn)

#include "template.glsl"
