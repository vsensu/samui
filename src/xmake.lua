add_requires("spdlog 1.8.5")
add_requires("glfw 3.3.2")

-- includes("log", "events")
-- add_requires("conan::imgui/1.78", {alias = "imgui"})

add_defines("SAMUI_BUILD_DLL")

if is_mode("debug") then
    add_defines("SAMUI_DEBUG")
elseif is_mode("release") then
    add_defines("SAMUI_RELEASE")
end

target("engine")
    set_kind("shared")
    set_pcxxheader("core/pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    add_includedirs("$(projectdir)/deps/src/imgui-1.83")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("**.cpp")
    add_packages("spdlog","glfw")
    add_deps("src_example", "glad")
    add_deps("imgui")
    add_links("lib_example")
