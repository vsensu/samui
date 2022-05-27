-- target("samui-editor")
--     set_kind("binary")
--     set_languages("c99", "cxx20")
--     add_defines("ENTT_API_IMPORT")
--     if is_mode("debug") then
--         add_defines("SAMUI_DEBUG")
--     elseif is_mode("release") then
--         add_defines("SAMUI_RELEASE")
--     end

--     add_includedirs("$(projectdir)/src")
--     add_includedirs("$(projectdir)/src/engine")
--     add_includedirs("$(projectdir)/src/editor")
--     add_includedirs("$(projectdir)/src/deps/entt-3.8.1/single_include")
--     add_includedirs("$(projectdir)/src/deps/glad/include")
--     add_includedirs("$(projectdir)/src/deps/imgui-docking")

--     add_files("**.cpp")
--     add_packages("spdlog", "glfw", "glm", "yaml-cpp", "asio")
--     add_deps("engine", "glad", "imgui")
--     -- add_links("user32", "gdi32", "shell32")

--     after_build(function (target)
--         local target_dir = path.join(target:targetdir(), "editor")
--         if not os.isdir(target_dir) then
--             os.mkdir(target_dir)
--         end
--         print("[build] copy editor assets ...")
--         os.cp("$(projectdir)/src/editor/assets/", target_dir)
--     end)
add_requires("yaml-cpp 0.6.3")

target("samui-editor")
    set_kind("binary")
    set_languages("c99", "cxx20")
    -- add_defines("ENTT_API_IMPORT")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_includedirs("$(projectdir)/src/deps/entt-3.8.1/single_include")
    add_includedirs("$(projectdir)/src")
    add_includedirs("$(projectdir)/src/editor")
    add_files("**.cpp")
    add_packages("spdlog", "glm", "glfw", "yaml-cpp")
    -- add_deps("samui-log", "samui-assert", "samui-profiler", "samui-core", "samui-console")
    add_deps("imgui", "samui-log", "samui-profiler", "samui-core", "samui-rendering", "samui-graphics")
    after_build(function (target)
        local target_dir = path.join(target:targetdir(), "editor")
        if not os.isdir(target_dir) then
            os.mkdir(target_dir)
        end
        print("[build] copy editor assets ...")
        os.cp("$(projectdir)/src/editor/assets/", target_dir)
    end)