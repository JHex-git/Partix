#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Partix
{

struct Vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
};

struct SubMesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

class Mesh
{
public:
    inline bool HasLoaded() const { return m_submeshes.size() != 0; }
    bool Load(const std::string& file_path);

    inline const std::vector<SubMesh> GetSubMeshes() const { return m_submeshes; }

private:
    void ProcessNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    SubMesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);

private:
    std::vector<SubMesh> m_submeshes;
};
} // namespace Partix
