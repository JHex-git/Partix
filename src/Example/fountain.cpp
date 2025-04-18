#include <vector>
#include <Renderer/Renderer.cpp>
#include <glad/glad.h>

int main()
{


    PartixRenderer::Renderer renderer(800, 800);
    renderer.Render();
    return 0;
}