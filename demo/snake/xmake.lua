target("snake")
    set_kind("binary")
    set_languages("c99", "cxx20")
    add_includedirs("$(projectdir)/src")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_files("*.cpp")
    add_packages("spdlog", "glm", "glfw")
    add_deps("imgui", "samui-log", "samui-profiler", "samui-core", "samui-rendering", "samui-graphics")
    after_build(function (target)
        local target_dir = target:targetdir()
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy snake assets ...")
        os.cp("$(projectdir)/demo/snake/assets/", target_dir)
    end)