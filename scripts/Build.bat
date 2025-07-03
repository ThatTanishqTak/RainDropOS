@echo off

pushd "../"

mkdir build
cd build
cmake ..
make

pause