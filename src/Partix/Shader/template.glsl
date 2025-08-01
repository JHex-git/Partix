#ifndef TEMPLATE_GLSL
#define TEMPLATE_GLSL
struct ParticleSystem
{
	#define PARTIX_MODULE(NAME) NAME	NAME##_module;
		PARTIX_MODULES
	#undef PARTIX_MODULE
};

void main()
{
	ParticleSystem particle_system;

	// Initialization Particles
	#define PARTIX_MODULE(NAME) NAME##_Init(particle_system.NAME##_module, particle);
		PARTIX_MODULES
	#undef PARTIX_MODULE

	if ( !particle.alive )
	{
		return;
	}

	// Update Particles
	#define PARTIX_MODULE(NAME) NAME##_Update(particle_system.NAME##_module, particle);
		PARTIX_MODULES
	#undef PARTIX_MODULE

	if ( !particle.alive )
	{
		return;
	}
}
#endif // TEMPLATE_GLSL