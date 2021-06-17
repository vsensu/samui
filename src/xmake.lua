add_defines("SAMUI_BUILD_DLL")

includes("log", "events", "platform")

target("engine")
    set_kind("shared")
    set_pcxxheader("pch.h")
    add_includedirs("$(projectdir)/deps/headeronly", "$(projectdir)/deps/libs", "$(projectdir)/deps/src")
    add_linkdirs("$(projectdir)/deps/libs/lib_example")
    add_files("*.cpp")
    add_packages("spdlog")
    add_deps("src_example", "log", "events", "platform")
    add_links("lib_example")
