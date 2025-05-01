#include <Partix/Emitter.h>
#include <Attrix/EmitterFactory.h>
#include <Attrix/DefaultEmitterDeserializer.h>

namespace Partix
{

namespace
{
struct Fountain
{
    float jitterAngleRange;
    float theta;
    alignas(16) glm::vec3 color;
};

struct FountainRegister
{
    FountainRegister()
    {
        Partix::EmitterFactory::Instance().RegisterEmitter("Fountain", sizeof(Partix::Emitter<Fountain>), [](tinyxml2::XMLElement *emitter_attributes_element)
        {
            Partix::Emitter<Fountain> *emitter = new Partix::Emitter<Fountain>();

            Partix::Emitter<DefaultAttributes>& default_part =*reinterpret_cast<Partix::Emitter<DefaultAttributes>*>(emitter);
            Partix::DefaultEmitterDeserializer(emitter_attributes_element, default_part);

            for (const auto *element = emitter_attributes_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
            {
                std::string element_type = element->Name();
                if (element_type == "JitterAngleRange")
                {
                    element->QueryFloatAttribute("Value", &emitter->attributes.jitterAngleRange);
                }
                else if (element_type == "Theta")
                {
                    element->QueryFloatAttribute("Value", &emitter->attributes.theta);
                }
                else if (element_type == "Color")
                {
                    element->QueryFloatAttribute("X", &emitter->attributes.color.x);
                    element->QueryFloatAttribute("Y", &emitter->attributes.color.y);
                    element->QueryFloatAttribute("Z", &emitter->attributes.color.z);
                }
            }

            return emitter;
        });
    }
} fountain_register;
}
} // Partix