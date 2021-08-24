-- project tool
target("samui")
    set_kind("binary")
    set_languages("c99", "cxx20")
    add_includedirs("$(projectdir)/src")
    add_includedirs("$(projectdir)/src/tools")
    add_files("**.cpp")
