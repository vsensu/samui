add_requires("spdlog 1.8.5")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")

target("2d")
    set_kind("binary")
    set_languages("c99", "cxx20")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    -- add_defines("ENTT_API_IMPORT")
    add_includedirs("$(env SAMUI)/0.1.0-alpha/include")
    add_includedirs("$(env SAMUI)/0.1.0-alpha/include/engine")
    add_includedirs("$(env SAMUI)/0.1.0-alpha/include/deps/imgui-docking")
    add_files("*.cpp")
    add_packages("spdlog", "glfw", "glm", "asio")
    add_linkdirs("$(env SAMUI)/0.1.0-alpha/lib/$(plat)/$(arch)/$(mode)")
    add_links("imgui", "samui-log", "samui-profiler", "samui-core", "samui-rendering", "samui-graphics")
    -- add_links("user32", "gdi32", "shell32")
    after_build(function (target)
        local target_dir = path.join(target:targetdir(), "engine")
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy engine assets ...")
        os.cp("$(env SAMUI)/0.1.0-alpha/engine/assets/", target_dir)
    end)
