#ifndef VIEW_GLSL
#define VIEW_GLSL

layout(std140, binding = 0) uniform View
{
    mat4 view_mat;
    mat4 projection_mat;
    float prevTime;
    float currentTime;
    float deltaTime;
} view;

#endif // VIEW_GLSL