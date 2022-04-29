add_requires("catch2 2.13.9")

target("test")
    set_kind("binary")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("test.cpp")
    add_packages("catch2", "freetype")
    add_deps("engine")
