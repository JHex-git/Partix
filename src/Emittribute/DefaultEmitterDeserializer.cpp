#include <iostream>
#include <string>
#include <Emittribute/DefaultEmitterDeserializer.h>
#include <Emittribute/EmitterFactory.h>

namespace Partix
{

void DefaultEmitterDeserializer(const tinyxml2::XMLElement *emitter_element, Emitter<DefaultAttributes> &emitter)
{
    for (const auto *element = emitter_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        std::string element_type = element->Name();
        if (element_type == "Position")
        {
            element->QueryFloatAttribute("X", &emitter.position.x);
            element->QueryFloatAttribute("Y", &emitter.position.y);
            element->QueryFloatAttribute("Z", &emitter.position.z);
        }
        else if (element_type == "Direction")
        {
            element->QueryFloatAttribute("X", &emitter.direction.x);
            element->QueryFloatAttribute("Y", &emitter.direction.y);
            element->QueryFloatAttribute("Z", &emitter.direction.z);
        }
        else if (element_type == "EmitCountPerFrame")
        {
            element->QueryIntAttribute("Value", &emitter.emitCountPerFrame);
        }
        else if (element_type == "Lifetime")
        {
            element->QueryFloatAttribute("Value", &emitter.lifetime);
        }
        else if (element_type == "EmitVelocity")
        {
            element->QueryFloatAttribute("Value", &emitter.emitVelocity);
        }
        else if (element_type == "SpriteSize")
        {
            element->QueryFloatAttribute("Value", &emitter.spriteSize);
        }
        else if (element_type == "MaxParticleCount")
        {
            element->QueryIntAttribute("Value", &emitter.maxParticleCount);
        }
    }
}

namespace
{
struct DefaultRegister
{
    DefaultRegister()
    {
        Partix::EmitterFactory::Instance().RegisterEmitter("Default", sizeof(Partix::Emitter<DefaultAttributes>), [](tinyxml2::XMLElement *emitter_element)
        {
            Partix::Emitter<DefaultAttributes> *emitter = new Partix::Emitter<DefaultAttributes>();
            Partix::DefaultEmitterDeserializer(emitter_element, *emitter);
            return emitter;
        });
    }
} default_register;
}
} // namespace Partix
