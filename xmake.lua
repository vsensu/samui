set_project("samui")
set_xmakever("2.3.2")
set_version("1.6.5", {build = "%Y%m%d%H%M"})

-- set_warnings("all", "error")
set_languages("c99", "c++20")
-- add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
-- add_mxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")

add_rules("mode.release", "mode.debug")

includes("src")