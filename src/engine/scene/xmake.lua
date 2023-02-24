add_requires("spdlog 1.8.5")
add_requires("glm 0.9.9+8")
add_requires("yaml-cpp 0.6.3")

target("samui-scene")
    set_kind("shared")
    set_languages("c99", "cxx20")
    add_defines("SAMUI_BUILD_DLL")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    add_includedirs("$(projectdir)/src/deps/entt-3.8.1/single_include")
    add_includedirs("$(projectdir)/src/engine", {public = true})
    -- set_pcxxheader("pch.h")
    add_files("**.cpp")
    add_packages("spdlog", "glm", "yaml-cpp")
    add_deps("samui-log")
