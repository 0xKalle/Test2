cmake_minimum_required(VERSION 2.8.12)

# Set a default build configuration if none is specified. 'MinSizeRel' produces the smallest binaries
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'MinSizeRel' as none was specified.")
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
# set LIS Solver include/lib directories
set(LIS_INCLUDE_DIR /usr/local/lis/include)
set(LIS_LIB_DIR /usr/local/lis/lib)
project(lis_wrapper)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
# OpenMP 
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
# Python includes/libs
find_package(PythonInterp 2.7 REQUIRED)
#find_package(PythonLibs 2.7 REQUIRED)
#message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
#message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
#message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
# LIS Solver
include_directories(${LIS_INCLUDE_DIR})
LINK_DIRECTORIES(${LIS_LIB_DIR})
LINK_LIBRARIES(lis)
# pybind11
add_subdirectory(pybind11)
pybind11_add_module(lis_wrapper lis.cpp)
pybind11_enable_warnings(lis_wrapper)

