-- project
set_project("2d-project")

-- set project version
set_version("1.0.0", {build = "%Y%m%d%H%M"})

-- set language
set_languages("c99", "cxx20")

add_rules("mode.debug", "mode.release", "mode.minsizerel")

if is_plat("windows") then
    add_defines("SAMUI_PLATFORM_WINDOWS")
end

-- include project sources
includes("src")
