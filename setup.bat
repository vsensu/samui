conan install . -s build_type=Debug -s arch=x86 -if build_debug
conan install . -s build_type=Release -s arch=x86 -if build_release
pause