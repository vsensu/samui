add_requires("spdlog 1.8.5")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")

target("2d")
    set_kind("binary")
    set_languages("c99", "cxx20")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_defines("ENTT_API_IMPORT")
    add_includedirs("$(env SAMUI)/include")
    add_includedirs("$(env SAMUI)/include/engine")
    add_includedirs("$(env SAMUI)/include/deps/entt-3.8.1/single_include")
    add_includedirs("$(env SAMUI)/include/deps/glad/include")
    add_includedirs("$(env SAMUI)/include/deps/imgui-docking")
    add_files("*.cpp")
    add_packages("spdlog", "glfw", "glm")
    add_linkdirs("$(env SAMUI)/lib")
    add_links("engine", "glad", "imgui")
    -- add_links("user32", "gdi32", "shell32")
    after_build(function (target)
        local target_dir = path.join(target:targetdir(), "engine")
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy engine assets ...")
        os.cp("$(env SAMUI)/engine/assets/", target_dir)
    end)
