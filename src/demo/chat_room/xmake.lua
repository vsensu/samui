add_requires("conan::entt/3.5.2", {alias = "entt"})
add_requires("conan::glfw/3.3.2", {alias = "glfw"})
add_requires("conan::glm/0.9.9.8", {alias = "glm"})
add_requires("conan::imgui/1.78", {alias = "imgui"})
add_requires("conan::enet/1.3.17", {alias = "enet"})
add_requires("sfml 2.5.1")

target("demo_chat_room_server")
    set_kind("binary")
    set_languages("c99", "c++20")
    add_includedirs("$(projectdir)/src")
    add_files("server.cpp")
    add_packages("enet", "sfml")
    if is_plat("windows") then 
        add_links("ws2_32", "winmm")
    end

target("demo_chat_room_client")
    set_kind("binary")
    set_languages("c99", "c++20")
    add_includedirs("$(projectdir)/src")
    add_files("client.cpp")
    add_packages("enet", "sfml")
    if is_plat("windows") then 
        add_links("ws2_32", "winmm")
    end