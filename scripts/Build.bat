@echo off

pushd ".."

if not exist build mkdir build
cd build
cmake ..  -DCMAKE_BUILD_TYPE=Debug
cmake --build .

popd
pause