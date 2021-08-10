-- project
set_project("samui_engine")

-- set project version
set_version("1.0.0", {build = "%Y%m%d%H%M"})

-- set language
set_languages("c99", "cxx20")

add_rules("mode.debug", "mode.release", "mode.minsizerel")

if is_plat("windows") then
    add_defines("SAMUI_PLATFORM_WINDOWS")
end

-- include project sources
includes("deps", "src", "test", "demo")

task("ie")
    on_run("scripts/tasks/init_env")
    set_menu {
        usage = "xmake ie",
        description = "init env!",
        options = {}
    } 

task("dc")
    on_run("scripts/tasks/deep_clean")
    set_menu {
        usage = "xmake dc",
        description = "deep clean",
        options = {}
    }

task("reset")
    on_run("scripts/tasks/reset")
    set_menu {
        usage = "xmake reset",
        description = "reset xmake env",
        options = {}
    }

task("vsc")
    on_run("scripts/tasks/vsc_cc")
    set_menu {
        usage = "xmake vsc",
        description = "generate compile_commands.json for vscode",
        options = {}
    }