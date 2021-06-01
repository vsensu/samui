## Platform
xmake f -p [windows|linux|macosx|android|iphoneos ..] -a [x86|arm64 ..] -m [debug|release]
> eg. xmake f -p windows -a x86 -m debug

## Build
xmake

## Demo
xmake run demo

## Test
xmake run test

## vscode compile_commands.json
xmake project -k compile_commands

C/C++: Edit Configurations (JSON) 

"configurations": [
    {
      "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
    }
  ],