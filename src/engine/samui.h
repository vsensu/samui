#ifndef SAMUI_SAMUI_H_
#define SAMUI_SAMUI_H_

// clang-format off

#include "core/core.h"
#include "core/application.h"
#include "core/keycodes.h"
#include "core/timestep.h"
#include "core/input.h"

#include "log/log.h"

#include "debug/instrumentor.h"

#include "events/application_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

#include "layer/layer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "imgui/imgui_layer.h"
#include "ImGuizmo/ImGuizmo.h"

#include "rendering/renderer.h"
#include "rendering/renderer2d.h"
#include "rendering/render_command.h"
#include "rendering/buffer.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/subtexture2d.h"
#include "rendering/frame_buffer.h"
#include "rendering/orthographic_camera.h"
#include "rendering/orthographic_camera_controller.h"

#include "scene/scene.h"
#include "scene/components.h"
#include "scene/serialization.h"

#include "utils/platform_utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// clang-format on

#endif