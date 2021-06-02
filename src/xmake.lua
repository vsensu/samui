add_defines("SAMUI_BUILD_DLL")

includes("log")

target("engine")
    set_kind("shared")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("*.cpp")
    add_deps("src_example", "log")
    add_links("lib_example")
