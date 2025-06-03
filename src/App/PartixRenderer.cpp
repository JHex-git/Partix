#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include <tinyxml2.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>
#include <Attrix/EmitterFactory.h>
#include <Attrix/ParticleFactory.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path to XML file>" << std::endl;
        return -1;
    }

    PartixRenderer::Renderer renderer(800, 800);
    PartixEngine partix_engine;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(argv[1]) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Failed to load XML file: " << argv[1] << std::endl;
        return -1;
    }
    std::filesystem::path xml_path(argv[1]);
    std::filesystem::path xml_directory = xml_path.parent_path();

    const tinyxml2::XMLElement* root = doc.RootElement();
    for (const tinyxml2::XMLElement* emitter_element = root->FirstChildElement(); emitter_element != nullptr; emitter_element = emitter_element->NextSiblingElement())
    {
        std::string emitter_element_name = emitter_element->Name();
        if (emitter_element_name != "Emitter")
        {
            std::cerr << "Unknown xml node " << emitter_element_name << std::endl;
        }
        else
        {
            std::string type_name = emitter_element->Attribute("Type");
            std::string particle_type = "Default";
            EmitterBase* emitter = nullptr;
            EmitterShaderInfo emitter_shader_info;

            for (const tinyxml2::XMLElement* element = emitter_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
            {
                std::string element_name = element->Name();
                if (element_name == "Attributes")
                {
                    emitter = Partix::EmitterFactory::Instance().CreateEmitter(type_name, const_cast<tinyxml2::XMLElement*>(element));
                }
                else if (element_name == "SimulateShader")
                {
                    emitter_shader_info.simulate_shader_path = element->Attribute("Path");
                }
                else if (element_name == "SpriteShader")
                {
                    emitter_shader_info.sprite_shader_path = element->Attribute("Path");
                }
                else if (element_name == "Textures")
                {
                    for (const tinyxml2::XMLElement *texture_element = element->FirstChildElement(); texture_element != nullptr; texture_element = texture_element->NextSiblingElement())
                    {
                        std::string texture = texture_element->Attribute("Path");
                        int binding = texture_element->IntAttribute("Binding");
                        texture = (xml_directory / texture).string();
                        emitter_shader_info.sprite_shader_texture_paths.push_back(std::move(texture));
                        emitter_shader_info.sprite_texture_bindings.push_back(binding);
                    }
                }
                else if (element_name == "Particle")
                {
                    const tinyxml2::XMLElement *particle_attribute_element = element->FirstChildElement();
                    particle_type = particle_attribute_element->Attribute("Type");
                }
                else if (element_name == "Mesh")
                {
                    std::string mesh_path = element->Attribute("Path");
                    emitter_shader_info.mesh = std::make_shared<Mesh>();
                    mesh_path = (xml_directory / mesh_path).string();
                    if (!emitter_shader_info.mesh->Load(mesh_path))
                    {
                        std::cerr << "Fail to load mesh " << mesh_path << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Unknown xml node " << element_name << " in emitter" << std::endl;
                }
            }

            int particle_size = Partix::ParticleFactory::Instance().GetParticleSize(particle_type);
            int particle_count = reinterpret_cast<Partix::Emitter<DefaultAttributes> *>(emitter)->maxParticleCount;
            void *particles_buffer = new char[particle_size * particle_count];
            Partix::ParticleFactory::Instance().CreateParticlesInBuffer(particle_type, particles_buffer, particle_count);
            Partix::ParticleFactory::Instance().InitializeParticlesSeedInBuffer(particle_type, particles_buffer, particle_count);

            const std::string &particle_fields = Partix::ParticleFactory::Instance().GetParticleFields(particle_type);
            emitter_shader_info.defines[EmitterShaderInfo::ParticleExtraAttributes] = particle_fields;
            partix_engine.AddEmitter(emitter, Partix::EmitterFactory::Instance().GetEmitterSize(type_name), emitter_shader_info, particles_buffer, particle_count, particle_size);

            delete emitter;
            delete[] static_cast<char *>(particles_buffer);
        }
    }

    renderer.Render(partix_engine);
}