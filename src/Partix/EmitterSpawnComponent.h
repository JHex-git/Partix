#pragma once
#include <Partix/Component.h>

namespace Partix
{

class EmitterSpawnComponent : public Component
{
public: 
    virtual void BuildData(DataBuildContext& context) override;
};
} // namespace Partix
