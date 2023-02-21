// clang-format off
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

#include "camera.h"
#include "model.h"
// clang-format on

class GameLayer : public samui::Layer
{
public:
    GameLayer()
    {
        samui::RenderCommand::set_flip_vertically_on_load(true);
        samui::RenderCommand::set_cull_face_enable(true);
        samui::RenderCommand::set_depth_test_enable(true);
        samui::RenderCommand::set_blend_func(
            samui::BlendFactor::Src_Alpha,
            samui::BlendFactor::One_Minus_Src_Alpha);
        samui::RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
        shader_ = samui::shader::create("assets/shaders/model_loading.glsl");
        texture_ = samui::texture2d::create("assets/textures/Checkerboard.png");

        // load models
        model_ = std::make_shared<Model>("assets/mesh/nanosuit/nanosuit.obj");

        camera_ =
            std::make_shared<samui::EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);
    }
    virtual void on_attach() override { SAMUI_INFO("game layer attach"); }

    virtual void on_update(const samui::Timestep& deltaTime) override
    {
        samui::RenderCommand::clear();
        camera_->OnUpdate(deltaTime);
        // Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
        // samui::Renderer2D::reset_stats();

        // glm::mat4 projection = glm::ortho(0.f, 1280.f, -720.f, 0.f,
        // -1.f, 1.f); samui::Renderer2D::begin_scene(projection);

        // samui::RenderCommand::set_depth_test_enable(true);
        // samui::RenderCommand::set_blend_enable(false);

        // samui::Renderer2D::draw_quad({32.f, -32.f}, {64.f, 64.f},
        //                              {1.0f, 0.0f, 0.0f, 1.0f});
        // samui::Renderer2D::draw_quad({200.f, -200.f}, {128.f, 128.f},
        // texture_);

        // samui::RenderCommand::set_depth_test_enable(false);
        // samui::RenderCommand::set_blend_enable(true);
        // samui::Renderer2D::draw_quad({48.f, -48.f}, {64.f, 64.f},
        //                              {0.0f, 1.0f, 0.0f, 0.3f});

        // samui::Renderer2D::end_scene();

        shader_->bind();
        shader_->set_mat4("view", camera_->GetViewMatrix());
        shader_->set_mat4("projection", camera_->GetProjection());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        shader_->set_mat4("model", model);
        shader_->set_float3("viewPos", camera_->GetPosition());

        // lighting
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        shader_->set_float3("light.position", lightPos);

        // light properties
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        shader_->set_float3("light.ambient", ambientColor);
        shader_->set_float3("light.diffuse", diffuseColor);
        shader_->set_float3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // material properties
        shader_->set_float3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader_->set_float3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader_->set_float3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f)); // specular lighting doesn't have full effect on this object's material
        shader_->set_float("material.shininess", 32.0f);

        model_->Draw(shader_);

        // float vertices[] = {
        //     0.5f,  0.5f,  0.0f,  // 右上角
        //     0.5f,  -0.5f, 0.0f,  // 右下角
        //     -0.5f, -0.5f, 0.0f,  // 左下角
        //     -0.5f, 0.5f,  0.0f   // 左上角
        // };

        // std::vector<Vertex> v;
        // for(int i = 0; i < 12; i+=3)
        // {
        //   Vertex x;
        //   x.position = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
        //   v.push_back(x);
        // }

        // unsigned int indices[] = {
        //     // 注意索引从0开始!
        //     // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        //     // 这样可以由下标代表顶点组合成矩形

        //     0, 1, 3,  // 第一个三角形
        //     1, 2, 3   // 第二个三角形
        // };

        // auto va = samui::vertex_array::create();
        // auto vb = samui::vertex_buffer::create(v.data(), v.size()*sizeof(Vertex));
        // vb->set_layout({
        //   {"Position", samui::ShaderDataType::Float3},
        //   {"Normal", samui::ShaderDataType::Float3},
        //   {"Texture", samui::ShaderDataType::Float2}
        // });
        // auto ib = samui::index_buffer::create(indices, sizeof(indices));
        // va->add_vertex_buffer(vb);
        // va->set_index_buffer(ib);
        // samui::RenderCommand::draw_indexed(va);
    }

    virtual void on_imgui_render() override {}

private:
    std::shared_ptr<samui::Shader>       shader_;
    std::shared_ptr<samui::Texture2D>    texture_;
    std::shared_ptr<Model>               model_;
    std::shared_ptr<samui::EditorCamera> camera_;
};

class Demo : public samui::GraphicsApplication
{
public:
    Demo() : GraphicsApplication(samui::WindowProps("Rendering"))
    {
        push_layer(new GameLayer());
    }
};

std::shared_ptr<samui::Application> samui::create_application()
{
    SAMUI_ENGINE_TRACE("engine trace");
    SAMUI_ENGINE_INFO("engine info");
    SAMUI_ENGINE_WARN("engine warn");
    SAMUI_ENGINE_ERROR("engine error");
    SAMUI_ENGINE_FATAL("engine fatal");

    SAMUI_TRACE("game trace");
    SAMUI_INFO("game info");
    SAMUI_WARN("game warn");
    SAMUI_ERROR("game error");
    SAMUI_FATAL("game fatal");

    return std::make_shared<Demo>();
}
