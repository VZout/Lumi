cmake_minimum_required(VERSION 3.9.0)

##### INCLUDE DIRECTORIES #####
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include_directories(../src)

##### COMMON MODULE #####
project(LumiCommon)

file(GLOB_RECURSE SOURCES "common/*.cpp")
file(GLOB_RECURSE HEADERS "common/*.hpp")

add_library(LumiCommon ${SOURCES} ${HEADERS})

source_group("Code" FILES ${SOURCES} ${HEADERS})

set_target_properties(LumiCommon PROPERTIES LINKER_LANGUAGE CXX)

##### Low Level MODULE #####
project(LumiLowLevel)

file(GLOB_RECURSE SOURCES "low_level/*.cpp")
file(GLOB_RECURSE HEADERS "low_level/*.hpp")

add_library(LumiLowLevel ${SOURCES} ${HEADERS})

source_group("Code" FILES ${SOURCES} ${HEADERS})

if(LUMI_PLATFORM_UWP)
	target_link_libraries(LumiLowLevel dxguid.lib d3d12.lib dxgi.lib d3dcompiler.lib)
endif()

##### High Level MODULE #####
project(LumiHighLevel)

file(GLOB_RECURSE SOURCES "high_level/*.cpp")
file(GLOB_RECURSE HEADERS "high_level/*.hpp")

add_library(LumiHighLevel ${SOURCES} ${HEADERS})

source_group("Code" FILES ${SOURCES} ${HEADERS})

if(LUMI_PLATFORM_UWP)
	target_link_libraries(LumiHighLevel dxguid.lib d3d12.lib dxgi.lib d3dcompiler.lib)
endif()
