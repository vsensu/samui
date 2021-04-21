add_requires("conan::enet/1.3.17")

target("test_enet_env")
    set_kind("binary")
    add_files("test_enet_env.cpp")
    add_packages("enet")
    if is_plat("windows") then 
        add_links("ws2_32", "winmm")
    end

target("test_enet_demo_server")
    set_kind("binary")
    add_files("test_enet_demo_server.cpp")
    add_packages("enet")
    if is_plat("windows") then 
        add_links("ws2_32", "winmm")
    end

target("test_enet_demo_client")
    set_kind("binary")
    add_files("test_enet_demo_client.cpp")
    add_packages("enet")
    if is_plat("windows") then 
        add_links("ws2_32", "winmm")
    end