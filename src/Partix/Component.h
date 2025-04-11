#pragma once
#include <Partix/DataBuildContext.h>

namespace Partix
{

class Component
{
public:
    virtual ~Component() = default;
    virtual void BuildData(DataBuildContext& context) = 0;
};
} // namespace Partix
