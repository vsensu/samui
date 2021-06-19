add_defines("SAMUI_BUILD_DLL")

includes("log", "events", "platform")

add_requires("imgui 1.82")

target("engine")
    set_kind("shared")
    set_pcxxheader("pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("*.cpp")
    add_packages("spdlog", "imgui", "glfw")
    add_deps("src_example", "glad", "log", "events", "platform")
    add_links("lib_example")
