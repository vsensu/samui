target("game")
    set_kind("binary")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src/engine/")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_files("*.cpp")
    add_packages("spdlog", "glfw", "glm")
    add_deps("engine", "glad", "imgui")
    -- add_links("user32", "gdi32", "shell32")
    after_build(function (target)
        local target_dir = target:targetdir()
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy game assets ...")
        os.cp("$(projectdir)/demo/game/assets/", target_dir)
    end)