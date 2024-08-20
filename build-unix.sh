mkdir -p intermediate

cmake -S . -B ./intermediate/unix -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build ./intermediate/unix

echo "Binary should be in ./binaries"