## Platform
xmake f -p [windows|linux|macosx|android|iphoneos ..] -a [x86|arm64 ..] -m [debug|release]
> eg. xmake f -p windows -a x86 -m debug

## Build
xmake

## vscode compile_commands.json
xmake project -k compile_commands

ctrl+shift+P打开Command Palette,运行 C/C++: Edit Configurations (JSON) 生成c_cpp_properties.json：

"configurations": [
    {
      "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
    }
  ],