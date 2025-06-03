#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <Partix/Mesh.h>

namespace Partix
{

bool Mesh::Load(const std::string& file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file_path, aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Failed to load mesh: " << importer.GetErrorString() << std::endl;
        return false;
    }

    ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f));
    std::cout << "Mesh " << file_path << " loaded successfully." << std::endl;
    return true;
}

void Mesh::ProcessNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
    auto node_transform = node->mTransformation;
    glm::mat4 node_transform_glm;
    node_transform_glm[0][0] = node_transform.a1;
    node_transform_glm[0][1] = node_transform.b1;
    node_transform_glm[0][2] = node_transform.c1;
    node_transform_glm[0][3] = node_transform.d1;
    node_transform_glm[1][0] = node_transform.a2;
    node_transform_glm[1][1] = node_transform.b2;
    node_transform_glm[1][2] = node_transform.c2;
    node_transform_glm[1][3] = node_transform.d2;
    node_transform_glm[2][0] = node_transform.a3;
    node_transform_glm[2][1] = node_transform.b3;
    node_transform_glm[2][2] = node_transform.c3;
    node_transform_glm[2][3] = node_transform.d3;
    node_transform_glm[3][0] = node_transform.a4;
    node_transform_glm[3][1] = node_transform.b4;
    node_transform_glm[3][2] = node_transform.c4;
    node_transform_glm[3][3] = node_transform.d4;

    glm::mat4 transform = parentTransform * node_transform_glm;
    // process all the node’s meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_submeshes.push_back(ProcessMesh(mesh, scene, transform));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, transform);
    }
}

SubMesh Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform)));
    // process vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position;
        glm::vec2 tex_coord;
        if (mesh->HasPositions())
        {
            position = transform * glm::vec4(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), 1.0f);
        }

        if (mesh->HasTextureCoords(0))
        {
            tex_coord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        vertices.push_back(Vertex{position, tex_coord});
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // process each face’s indices
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return SubMesh{std::move(vertices), std::move(indices)};
}
} // namespace Partix
