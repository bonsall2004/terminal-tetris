@echo off


cmake -S . -B ./intermediate/win32 -G "Ninja" -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build ./intermediate/win32
echo "Binary should be in ./binaries"