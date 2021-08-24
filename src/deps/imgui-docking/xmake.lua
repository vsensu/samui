target("imgui")
    set_kind("shared")
    add_defines("IMGUI_BUILD_DLL")
    add_files("**.cpp")
