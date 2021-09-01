## Platform
xmake f -p [windows|linux|macosx|android|iphoneos ..] -a [x86|arm64 ..] -m [debug|release]
> eg. xmake f -p windows -a x86 -m debug

## Build
xmake

## Demo
xmake run game

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

## clang-format
clang-format -style=Google -dump-config > .clang-format

**turn off**

``` c++
// clang-format off
    void    unformatted_code  ;
// clang-format on
```

## Generate Visual Studio Project

```
xmake project -k [vsxmake2010|vsxmake2013|vsxmake2015|..] -m "debug;release"
xmake project -k vsxmake -m "debug;release"
```