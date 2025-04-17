#include <vector>
#include <Renderer/Renderer.cpp>
#include <glad/glad.h>

int main()
{


    PartixRenderer::Renderer renderer(200, 200);
    renderer.Render();
    return 0;
}