








#version 450













struct Particle
{
    bool alive;
    float lifetime;
    vec3 position;
    vec3 velocity;
};

layout(location=0) in Particle particle;


struct ParticleSystem
{
	
		EmitterSpawn EmitterSpawn_module;
	
};

void main()
{
	ParticleSystem particle_system;

	
	
		EmitterSpawn_Init(particle_system.EmitterSpawn_module, particle);
	

	if ( !particle.alive )
	{
		return;
	}

	
	
		EmitterSpawn_Update(particle_system.EmitterSpawn_module, particle);
	

	if ( !particle.alive )
	{
		return;
	}
}


