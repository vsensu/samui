add_requires("conan::enet/1.3.17", {alias = "enet"})
add_requires("sfml 2.5.1")

target("network")
    set_kind("static")
    add_packages("enet", "sfml")
