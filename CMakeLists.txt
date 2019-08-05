cmake_minimum_required(VERSION 3.0)

project(setsuna)

# Turn on the ability to create folders to organize projects (e.g. .vcproj)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(cotire OPTIONAL)

# Build mode
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type (Debug, Release, RelWithDebInfo, or MinSizeRel)")

# Build options
set(SETSUNA_BUILD_LOADERS OFF CACHE BOOL "Choose whether to build setsuna_loaders or not")
set(SETSUNA_BUILD_APPLICATIONS OFF CACHE BOOL "Choose whether to build applications or not")

# Dependencies
set(GLM_ROOT_DIR "" CACHE PATH "Root library directory of GLM")
set(GLAD_ROOT_DIR "" CACHE PATH "Root source directory of GLAD")

# Compiler options
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

    # Set warning level
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

	# Set the precision of floating-point calculations
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise")

	# Disable C++ RTTI
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

	# If the build type is Debug, define macro
	if(CMAKE_BUILD_TYPE STREQUAL Debug)
		add_definitions(-D_DEBUG)
	endif()

	# Set warning level
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")

	# Disable C++ RTTI
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Core library
add_subdirectory(setsuna_core)

# Resource loaders
if(SETSUNA_BUILD_LOADERS)
	set(STB_ROOT_DIR "" CACHE PATH "Root library directory of stb (required by setsuna_loaders)")
	add_subdirectory(setsuna_loaders)
endif()

# Applications
if(SETSUNA_BUILD_APPLICATIONS)
	set(GLFW_ROOT_DIR "" CACHE PATH "Root library directory of GLFW")
	set(GLFW_USE_STATIC_LIBS ON CACHE BOOL "Specifies to use static version of GLFW library (Windows only)")
	add_subdirectory(applications)
endif()
