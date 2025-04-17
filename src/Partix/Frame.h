#pragma once

namespace Partix
{

struct alignas(16) Frame
{
    float prevTime;
    float currentTime;
    float deltaTime;
};
} // namespace Partix
