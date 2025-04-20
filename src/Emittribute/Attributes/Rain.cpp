#include <Partix/Emitter.h>
#include <Emittribute/EmitterFactory.h>
#include <Emittribute/DefaultEmitterDeserializer.h>

namespace Partix
{

namespace
{
struct Rain
{
    alignas(8) glm::vec2 cover;
    float randomTheta;
};

struct RainRegister
{
    RainRegister()
    {
        Partix::EmitterFactory::Instance().RegisterEmitter("Rain", sizeof(Partix::Emitter<Rain>), [](tinyxml2::XMLElement *emitter_attributes_element)
        {
            Partix::Emitter<Rain> *emitter = new Partix::Emitter<Rain>();

            Partix::Emitter<DefaultAttributes>& default_part =*reinterpret_cast<Partix::Emitter<DefaultAttributes>*>(emitter);
            Partix::DefaultEmitterDeserializer(emitter_attributes_element, default_part);

            for (const auto *element = emitter_attributes_element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
            {
                std::string element_type = element->Name();
                if (element_type == "Cover")
                {
                    element->QueryFloatAttribute("X", &emitter->attributes.cover.x);
                    element->QueryFloatAttribute("Y", &emitter->attributes.cover.y);
                }
                else if (element_type == "RandomTheta")
                {
                    element->QueryFloatAttribute("Value", &emitter->attributes.randomTheta);
                }
            }

            return emitter;
        });
    }
} rain_register;
}
} // Partix