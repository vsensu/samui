add_requires("catch2")

target("test")
    set_kind("binary")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("test.cpp")
    add_packages("catch2")
    add_deps("core")
