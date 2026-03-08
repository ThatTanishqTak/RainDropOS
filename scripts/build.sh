cd ..
rm -rf build && cmake -B build -S . && cmake --build build && ./build/src/shell/raindrop-shell