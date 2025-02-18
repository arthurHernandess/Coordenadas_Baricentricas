@ECHO OFF
del CMakeCache.txt
".compilers/CMake/bin/cmake" .
".compilers/CMake/bin/cmake" --build .
"Debug/Barycentric.exe"
PAUSE
