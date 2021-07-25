add_requires("spdlog 1.8.5")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")

-- includes("log", "events")
-- add_requires("conan::imgui/1.78", {alias = "imgui"})

target("engine")
    set_kind("shared")
    add_defines("SAMUI_BUILD_DLL", "SAMUI_RENDER_OPENGL")
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    set_pcxxheader("core/pch.h")
    add_includedirs("$(projectdir)/src")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_includedirs("$(projectdir)/deps/src/stb_image")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("**.cpp")
    add_packages("spdlog","glfw", "glm")
    add_deps("src_example", "glad", "stb_image")
    add_deps("imgui")
    add_links("lib_example")
