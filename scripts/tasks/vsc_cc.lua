function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task
    if not os.exists(".vscode") then
        print("no vscode env!")
        return
    end

    print("remove last generated file...")
    os.tryrm("compile_commands.json")
    print("generate file...")
    os.exec("xmake project -k compile_commands")
    print("move file...")
    os.tryrm(".vscode/compile_commands.json")
    os.trymv("compile_commands.json", ".vscode/compile_commands.json")

    local ccpp_props = format(
[[
{
    "configurations": [
        {
            "name": "windows_engine_debug",
            "defines": [
                "SAMUI_DEBUG",
                "SAMUI_PLATFORM_WINDOWS",
                "SAMUI_BUILD_DLL"
            ],
            "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
        },
        {
            "name": "windows_engine_release",
            "defines": [
                "SAMUI_RELEASE",
                "SAMUI_PLATFORM_WINDOWS",
                "SAMUI_BUILD_DLL"
            ],
            "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
        },
        {
            "name": "windows_game_debug",
            "defines": [
                "SAMUI_DEBUG",
                "SAMUI_PLATFORM_WINDOWS"
            ],
            "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
        },
        {
            "name": "windows_game_release",
            "defines": [
                "SAMUI_RELEASE",
                "SAMUI_PLATFORM_WINDOWS"
            ],
            "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
        }
    ]
}
]])
    io.writefile(".vscode/c_cpp_properties.json", ccpp_props)
end
