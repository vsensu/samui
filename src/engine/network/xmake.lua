add_requires("conan::enet/1.3.17", {alias = "enet"})

target("network")
    set_kind("static")
    add_packages("enet", "sfml")
