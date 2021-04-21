add_defines("ENGINE")

add_requires("entt 3.7.0")
add_requires("glfw 3.3.2")
add_requires("glm 0.9.9+8")
add_requires("imgui 1.82")

-- includes("network")

target("engine")
    set_kind("binary")
    add_includedirs("$(projectdir)/deps/glad/include")
    add_includedirs("$(projectdir)/deps/imgui/platforms/include")
    add_files("$(projectdir)/deps/**/*.cpp")
    add_files("$(projectdir)/deps/**/*.c")
    add_files("./**/*.cpp")
    add_files("*.cpp")
    if is_plat("windows") then 
        add_ldflags("-subsystem:windows", "-entry:mainCRTStartup", {force = true}) 
        add_links("user32", "gdi32", "shell32")
    end
    add_packages("entt", "glfw", "glm", "imgui")