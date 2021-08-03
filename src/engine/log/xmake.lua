add_requires("spdlog 1.8.5")

target("log")
    set_kind("shared")
    set_pcxxheader("../pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("*.cpp")
    add_packages("spdlog")