@echo off
set prefix=[INSTALLER]

:inst_deps
echo %prefix% Installing dependencies...

::call git clone https://github.com/VZout/assimp.git deps/assimp
::call git clone https://github.com/glfw/glfw deps/glfw
::call git clone https://github.com/USCiLab/cereal.git deps/cereal

echo %prefix% Finished isntalling dependencies

::mkdir build-win64
::cd build-win64

::echo %prefix% Initializing CMake (VS2017 Win64)...
::call cmake -G "Visual Studio 15 2017 Win64" ..

::cd ..

mkdir build_uwp64
cd build_uwp64

echo %prefix% Initializing CMake (VS2017 Universal Windows Platform Win64)...
call CMake -G "Visual Studio 15 2017 Win64" -DLUMI_PLATFORM_UWP=ON -D CMAKE_SYSTEM_NAME:STRING=WindowsStore -D CMAKE_SYSTEM_VERSION:STRING=10.0 ..

pause
EXIT /B 0
