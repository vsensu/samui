add_defines("SAMUI_BUILD_DLL")

add_requires("glfw 3.3.2")

includes("log", "events")
-- add_requires("conan::imgui/1.78", {alias = "imgui"})

target("engine")
    set_kind("shared")
    set_pcxxheader("pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_includedirs("$(projectdir)/deps/src/imgui-1.83")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("*.cpp")
    add_packages("spdlog","glfw")
    add_deps("src_example", "glad", "log", "events")
    add_deps("imgui")
    add_links("lib_example")
