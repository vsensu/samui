target("editor")
    set_kind("binary")
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src/engine/")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_files("*.cpp")
    add_packages("spdlog", "glfw", "glm", "entt")
    add_deps("engine", "glad", "imgui")
    -- add_links("user32", "gdi32", "shell32")