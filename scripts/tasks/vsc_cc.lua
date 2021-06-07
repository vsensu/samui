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
    os.tryrm(".vscode/c_cpp_properties.json")
    os.trycp("scripts/tasks/c_cpp_properties.json", ".vscode/c_cpp_properties.json")
end
