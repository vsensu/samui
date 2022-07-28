target("samui-collision")
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
    add_includedirs("$(projectdir)/src/engine", {public = true})
    add_files("**.cpp")
    add_headerfiles("**.h")
    add_deps("samui-basic")
