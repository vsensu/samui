add_requires("spdlog 1.8.5")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")
add_requires("yaml-cpp 0.6.3")
add_requires("freetype 2.10.4")
add_requires("asio 1.20.0")
-- add_requires("entt 3.7.1")

-- includes("log", "events")
-- add_requires("conan::imgui/1.78", {alias = "imgui"})

target("engine")
    set_kind("shared")
    set_languages("c99", "cxx20")
    add_defines("SAMUI_BUILD_DLL", "SAMUI_RENDER_OPENGL", "ENTT_API_EXPORT")
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    set_pcxxheader("core/pch.h")
    add_includedirs("$(projectdir)/src")
    add_includedirs("$(projectdir)/src/engine")
    add_includedirs("$(projectdir)/src/deps/entt-3.8.1/single_include")
    add_includedirs("$(projectdir)/src/deps/glad/include")
    add_includedirs("$(projectdir)/src/deps/stb_image")
    add_includedirs("$(projectdir)/src/deps/imgui-docking")
    add_files("**.cpp")
    add_packages("spdlog","glfw", "glm", "yaml-cpp", "freetype", "asio")
    add_deps("glad", "stb_image", "imgui")
    if is_plat("windows") then 
        add_links("Comdlg32")
    end
    after_build(function (target)
        local target_dir = path.join(target:targetdir(), "engine")
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy engine assets ...")
        os.cp("$(projectdir)/src/engine/assets/", target_dir)
    end)
