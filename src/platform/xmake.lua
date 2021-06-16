add_requires("glfw 3.3.4")
add_requires("glad 0.1.34")

target("platform")
    set_kind("shared")
    set_pcxxheader("../pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src", "$(projectdir)/src")
    if is_plat("windows") then
        add_files("windows/**.cpp")
    end
    add_packages("glfw", "glad", "spdlog")
    add_deps("log")