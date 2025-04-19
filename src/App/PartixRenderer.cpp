#include <vector>
#include <glad/glad.h>
#include <tinyxml2.h>
#include <Renderer/Renderer.cpp>
#include <Partix/Emitter.h>
#include <Partix/PartixEngine.h>
#include <Emittribute/EmitterFactory.h>

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
            EmitterBase* emitter = nullptr;
            std::string simulate_shader;
            std::string sprite_shader;
            std::vector<std::string> textures;
            std::vector<int> texture_bindings;

            for (const tinyxml2::XMLElement* element = emitter_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
            {
                std::string element_name = element->Name();
                if (element_name == "Attributes")
                {
                    emitter = Partix::EmitterFactory::Instance().CreateEmitter(type_name, const_cast<tinyxml2::XMLElement*>(element));
                }
                else if (element_name == "SimulateShader")
                {
                    simulate_shader = element->Attribute("Path");
                }
                else if (element_name == "SpriteShader")
                {
                    sprite_shader = element->Attribute("Path");
                }
                else if (element_name == "Textures")
                {
                    for (const tinyxml2::XMLElement *texture_element = element->FirstChildElement(); texture_element != nullptr; texture_element = texture_element->NextSiblingElement())
                    {
                        std::string texture = texture_element->Attribute("Path");
                        int binding = texture_element->IntAttribute("Binding");
                        textures.push_back(std::move(texture));
                        texture_bindings.push_back(binding);
                    }
                }
                else
                {
                    std::cerr << "Unknown xml node " << element_name << " in emitter" << std::endl;
                }
            }

            partix_engine.AddEmitter(emitter, Partix::EmitterFactory::Instance().GetEmitterSize(type_name), simulate_shader, sprite_shader, textures, texture_bindings);
        }
    }

    renderer.Render(partix_engine);
}