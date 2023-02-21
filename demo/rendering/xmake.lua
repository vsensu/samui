add_requires("assimp 5.2.5")

target("rendering")
    set_kind("binary")
    set_languages("c20", "cxx20")
    add_includedirs("$(projectdir)/src")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_files("*.cpp")
    add_packages("spdlog", "glm", "glfw", "assimp")
    add_deps("imgui", "samui-log", "samui-profiler", "samui-core", "samui-rendering", "samui-graphics")
    after_build(function (target)
        local target_dir = target:targetdir()
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy game assets ...")
        os.cp("$(projectdir)/demo/rendering/assets/", target_dir)
    end)