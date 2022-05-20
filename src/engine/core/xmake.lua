add_requires("spdlog 1.8.5")

target("samui-core")
    set_kind("shared")
    set_languages("c99", "c++20")
    add_defines("SAMUI_BUILD_DLL")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    if is_mode("debug") then
        add_defines("SAMUI_DEBUG")
    elseif is_mode("release") then
        add_defines("SAMUI_RELEASE")
    end
    add_includedirs("$(projectdir)/src/engine", {public = true})
    add_files("**.cpp")
    add_packages("spdlog")
    add_deps("samui-log")