project(Lumi)

cmake_minimum_required(VERSION 3.9.0)

##### Compile Details ####
option(LUMI_BUILD_TESTS "Build Lumi tests." ON)
option(LUMI_PLATFORM_UWP "Compile for Universal Windows Platform." OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (LUMI_PLATFORM_UWP)
	set_property(GLOBAL PROPERTY LINK_FLAGS /WINMD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ZW")
	add_compile_options(/wd4146 /wd4996 /wd4703)
	add_definitions(-DPLATFORM_UWP)
endif()

add_compile_options(/bigobj)

##### COMPILER SETTINGS #####
set (CMAKE_CXX_STANDARD 17)
message(STATUS "Using C++"  ${CMAKE_CXX_STANDARD})

##### OUTPUT DIRECTORIES #####
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##### DEFINITIONS #####
add_definitions(-DNOMINMAX)

##### INCLUDE/LINK DIRECTORIES #####
if (USE_UWP)
	link_directories(
		${CMAKE_BINARY_DIR}/lib/
		${CMAKE_BINARY_DIR}/deps/glfw/src
	)
endif()

if (USE_UWP)
	include_directories(
		modules/
		deps/assimp/include/
		deps/cereal/include/	
		${CMAKE_BINARY_DIR}/deps/assimp/include
	)
endif()

##### SRC/TESTS SUB-DIRECTORIES #####
add_subdirectory(src)

if (LUMI_BUILD_TESTS)
	add_subdirectory(tests)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Window)
endif()

