#pragma once

// clang-format off
#include <vector>

#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/renderer/render_command.h>
#include <engine/graphics/renderer/texture_create.h>
#include <engine/graphics/renderer/shader_create.h>
#include <engine/graphics/renderer/buffer_create.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// clang-format on

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};

struct Texture
{
    std::shared_ptr<samui::Texture2D> texture;
    std::string                       type;
    aiString                          path;
};

class Mesh
{
public:
    /*  网格数据  */
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    /*  函数  */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }

    void Draw(std::shared_ptr<samui::Shader> shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        shader->bind();
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            textures[i].texture->bind(i);
            // 获取纹理序号（diffuse_textureN 中的 N）
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader->set_int((name + number).c_str(), i);
        }
        shader->set_int("DiffuseNum", diffuseNr);
        shader->set_int("SpecularNum", specularNr);
        samui::RenderCommand::draw_indexed(va_);
    }

private:
    /*  渲染数据  */
    std::shared_ptr<samui::VertexArray> va_;

    /*  函数  */
    void setupMesh()
    {
        auto vb = samui::vertex_buffer::create(
            vertices.data(), vertices.size() * sizeof(Vertex));
        vb->set_layout({{"Position", samui::ShaderDataType::Float3},
                        {"Normal", samui::ShaderDataType::Float3},
                        {"TexCoord", samui::ShaderDataType::Float2}});

        va_ = samui::vertex_array::create();
        va_->add_vertex_buffer(vb);

        auto ib = samui::index_buffer::create(indices.data(), indices.size());
        va_->set_index_buffer(ib);
    }
};

class Model
{
public:
    /*  函数   */
    Model(std::string path) { loadModel(path); }
    void Draw(std::shared_ptr<samui::Shader> shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
    }

private:
    /*  模型数据  */
    std::vector<Mesh>    meshes;
    std::string          directory;
    std::vector<Texture> textures_loaded;

    /*  函数   */
    void loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene*   scene =
            import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
        {
            SAMUI_ERROR("ERROR::ASSIMP::{}", import.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 接下来对它的子节点重复这一过程
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // 处理顶点位置、法线和纹理坐标
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            if (mesh->mNormals != nullptr)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }

            if (mesh->mTextureCoords[0])  // 网格是否有纹理坐标？
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texcoords = vec;
            }
            else
                vertex.texcoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // 处理索引
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // 处理材质
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(
                material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(),
                            diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(
                material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(),
                            specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial*   mat,
                                              aiTextureType type,
                                              std::string   typeName)
    {
        std::vector<Texture> textures;
        auto tex_count = mat->GetTextureCount(type);
        for (unsigned int i = 0; i < tex_count; i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) ==
                    0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {  // 如果纹理还没有被加载，则加载它
                Texture texture;
                // texture.id = TextureFromFile(str.C_Str(), directory);
                auto tex_path = std::filesystem::path(directory).append(str.C_Str());
                SAMUI_INFO("texture:{}", tex_path.string());
                texture.texture = samui::texture2d::create(tex_path);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // 添加到已加载的纹理中
            }
        }
        return textures;
    }
};