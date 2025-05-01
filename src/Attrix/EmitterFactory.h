#pragma once
#include <map>
#include <string>
#include <functional>
#include <tinyxml2.h>

namespace Partix
{
struct EmitterBase;

class EmitterFactory
{
public:
    static EmitterFactory &Instance()
    {
        static EmitterFactory instance;
        return instance;
    }

    EmitterFactory(const EmitterFactory &) = delete;
    EmitterFactory &operator=(const EmitterFactory &) = delete;
    EmitterFactory(EmitterFactory &&) = delete;
    EmitterFactory &operator=(EmitterFactory &&) = delete;

    EmitterBase *CreateEmitter(const std::string &type_name, tinyxml2::XMLElement *emitter_attributes_element);

    int GetEmitterSize(const std::string &type_name);

    using EmitterConstructor = std::function<EmitterBase *(tinyxml2::XMLElement *emitter_element)>;

    void RegisterEmitter(const std::string &type_name, int emitter_size, EmitterConstructor&& constructor);

private:
    EmitterFactory() = default;
    ~EmitterFactory() = default;

private:
    std::map<std::string, EmitterConstructor> m_emitter_constructors;
    std::map<std::string, int> m_emitter_sizes;
};
} // namespace Partix
