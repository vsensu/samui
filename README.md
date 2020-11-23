## First time
setup.bat

## Make
make_debug.bat

## Build
build_debug.bat

## vscode compile_commands.json
xmake project -k compile_commands
C/C++: Edit Configurations (JSON) 

"configurations": [
    {
      "compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
    }
  ],