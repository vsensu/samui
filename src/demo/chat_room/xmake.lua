add_requires("entt 3.7.0")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")
add_requires("imgui 1.82")
add_requires("conan::enet/1.3.17")
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
    add_includedirs("$(projectdir)/deps/glad/include")
    add_includedirs("$(projectdir)/deps/imgui/platforms/include")
    add_includedirs("$(projectdir)/src")
    add_files("$(projectdir)/deps/**/*.cpp")
    add_files("$(projectdir)/deps/**/*.c")
    add_files("$(projectdir)/src/engine/**/*.cpp")
    add_files("client.cpp")
    add_packages("enet", "sfml", "glfw", "glm", "imgui")
    if is_plat("windows") then 
        add_ldflags("-subsystem:windows", "-entry:mainCRTStartup", {force = true}) 
        add_links("ws2_32", "winmm")
        add_links("user32", "gdi32", "shell32")
    end