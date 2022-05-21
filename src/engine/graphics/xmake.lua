add_requires("glfw 3.3.2")

target("samui-graphics")
    set_kind("shared")
    set_languages("c99", "c++20")
    add_defines("SAMUI_BUILD_DLL", "SAMUI_RENDER_OPENGL")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    add_includedirs("$(projectdir)/src/deps/glad/include", {public = true})
    add_includedirs("$(projectdir)/src/deps/stb_image", {public=true})
    add_includedirs("$(projectdir)/src/deps/imgui-docking", {public = true})
    add_includedirs("$(projectdir)/src/engine", {public = true})
    add_files("*.cpp")
    if is_plat("windows") then
        add_files("windows/**.cpp")
    end
    add_files("backend/opengl/**.cpp")
    add_files("renderer/**.cpp")
    add_files("imgui_layer/**.cpp")
    add_files("ImGuizmo/**.cpp")
    add_packages("spdlog", "glm", "glfw")
    add_deps("glad", "stb_image", "imgui", "samui-log", "samui-profiler", "samui-core", "samui-rendering")