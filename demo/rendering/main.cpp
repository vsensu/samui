// clang-format off
#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/renderer/render_command.h>
#include <engine/graphics/renderer/texture_create.h>
#include <engine/graphics/renderer/shader_create.h>
#include <engine/graphics/renderer/buffer_create.h>
#include <engine/graphics/utils/platform_utils.h>
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

        shader_->bind();
        shader_->set_mat4("view", camera_->GetViewMatrix());
        shader_->set_mat4("projection", camera_->GetProjection());

        glm::mat4 model = glm::mat4(1.0f);

        // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

        // it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

        shader_->set_mat4("model", model);
        shader_->set_float3("viewPos", camera_->GetPosition());

        // lighting
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        shader_->set_float3("light.position", lightPos);

        // light properties
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        // decrease the influence
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        // low influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        shader_->set_float3("light.ambient", ambientColor);
        shader_->set_float3("light.diffuse", diffuseColor);
        shader_->set_float3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // material properties
        shader_->set_float3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader_->set_float3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        // specular lighting doesn't have full effect on this object's material
        shader_->set_float3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader_->set_float("material.shininess", 32.0f);

        model_->Draw(shader_);
    }

    virtual void on_imgui_render() override
    {
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGui::Begin("Menu", 0, window_flags);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    auto filepath =
                        samui::DialogUtils::open_file("Model File(*.*)\0*.*\0");
                    if (!filepath.empty())
                    {
                        model_ = std::make_shared<Model>(filepath);
                    }
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

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
