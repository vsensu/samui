add_requires("catch2 2.13.9")
add_requires("freetype 2.10.4")

target("test")
    set_kind("binary")
    set_languages("c99", "cxx20")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("test.cpp")
    add_packages("catch2", "freetype")
    add_deps("samui-serialization")

target("test-freetype")
    set_kind("binary")
    set_languages("c99", "cxx20")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("freetype.cpp")
    add_packages("catch2", "freetype")

target("test-serialization")
    set_kind("binary")
    set_languages("c99", "cxx20")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("serialization.cpp")
    add_packages("catch2")
    add_deps("samui-serialization")
