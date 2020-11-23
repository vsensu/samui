add_requires("conan::asio/1.18.0", {alias = "asio"})

target("test_asio")
    set_kind("binary")
    add_files("test_asio.cpp")
    add_packages("asio")