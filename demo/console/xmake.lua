target("console")
    set_kind("binary")
    set_languages("c99", "cxx20")
    add_includedirs("$(projectdir)/src")
    add_includedirs("$(projectdir)/src/engine")
    if is_plat("windows") then
        add_defines("SAMUI_PLATFORM_WINDOWS")
    end
    add_files("*.cpp")
    add_packages("spdlog")
    -- add_deps("samui-log", "samui-assert", "samui-profiler", "samui-core", "samui-console")
    add_deps("samui-console")
    after_build(function (target)
        -- local target_dir = target:targetdir()
        -- if not os.isdir(target_dir) then
        --     os.mkdir(target_dir)
        -- end
        -- print("[build] copy game assets ...")
        -- os.cp("$(projectdir)/demo/console/assets/", target_dir)
    end)