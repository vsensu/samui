add_requires("conan::asio/1.18.0", {alias = "asio"})

target("network")
    set_kind("static")
    add_files("*.cpp")
    add_packages("asio")