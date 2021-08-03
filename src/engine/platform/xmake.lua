add_requires("glfw 3.3.2")

target("platform")
    set_kind("shared")
    set_pcxxheader("../pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_includedirs("$(projectdir)/deps/src/glad/include")
    if is_plat("windows") then
        add_files("windows/**.cpp")
    end
    add_packages("glfw", "spdlog")
    add_deps("log", "glad")