target("demo")
    set_kind("binary")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/src", "$(projectdir)/src")
    add_files("*.cpp")
    add_deps("engine")