#pragma once

#include <tinyxml2.h>
#include <Partix/Emitter.h>

namespace Partix
{
    
void DefaultEmitterDeserializer(const tinyxml2::XMLElement *emitter_attributes_element, Emitter<DefaultAttributes> &emitter);

} // namespace Partix
