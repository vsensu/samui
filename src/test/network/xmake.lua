add_requires("conan::asio/1.18.0", {alias = "asio"})
add_requires("sfml 2.5.1")

target("test_asio")
    set_kind("binary")
    add_files("test_asio.cpp")
    add_packages("asio")

target("test_msg")
    set_kind("binary")
    add_includedirs("$(projectdir)/src")
    add_files("test_msg.cpp")

target("test_tsqueue")
    set_kind("binary")
    add_includedirs("$(projectdir)/src")
    add_files("test_tsqueue.cpp")

target("test_net")
    set_kind("binary")
    add_includedirs("$(projectdir)/src")
    add_files("test_net.cpp")
    add_packages("asio")

target("test_net_server")
    set_kind("binary")
    add_includedirs("$(projectdir)/src")
    add_files("test_net_server.cpp")
    add_packages("asio")

target("test_net_client")
    set_kind("binary")
    add_includedirs("$(projectdir)/src")
    add_files("test_net_client.cpp")
    add_packages("asio")

includes("enet")
