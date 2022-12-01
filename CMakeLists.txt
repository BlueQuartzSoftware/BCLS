
if(${CMAKE_MAJOR_VERSION} STREQUAL "3")
  # CMake 3.0 introduces VERSION variables for each Project. We do our own version numbering so we need to have
  # cmake ignore that policy
  cmake_policy(SET CMP0048 NEW)
  # CMake 3.0 introduces some OS X RPath policies
  cmake_policy(SET CMP0042 NEW)
endif()

cmake_minimum_required (VERSION 3.8.0)
project (BCLSProj LANGUAGES C CXX VERSION 0.1.0.0)

include(CMakeParseArguments)

# ---------- Setup output Directories -------------------------
if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${BCLSProj_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all Libraries"
    )
endif()

# --------- Setup the Executable output Directory -------------
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${BCLSProj_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all Executables."
    )
endif()

# --------- Setup the Executable output Directory -------------
if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${BCLSProj_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all static libraries."
    )
endif()

#Enable use of Solution Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Build shared libraries
OPTION(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
if(WIN32)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
endif()
set(LIB_TYPE SHARED)
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
    list(APPEND CMP_LIB_SEARCH_DIRS  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    if(MSVC)
        list(APPEND CMP_LIB_SEARCH_DIRS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
                                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release )
    endif()
endif(BUILD_SHARED_LIBS)


if (NOT DEFINED CMP_SOURCE_DIR)
  set(CMP_SOURCE_DIR ${BCLSProj_SOURCE_DIR}/Support/cmp)
endif()
include (${CMP_SOURCE_DIR}/cmpCMakeMacros.cmake )
include (${CMP_SOURCE_DIR}/cmpProject.cmake)


set(EXE_DEBUG_EXTENSION "_debug")

option (BCLS_USE_MKL "Use MKL for BLAS/LAPACK functions" OFF)
#------------------------------------------------------------------------------
# Find the Intel Math Kernel Library (MKL) which has BLAS/LAPACK functions
# 
if (BCLS_USE_MKL)
  # Define the interface layers and link type for MKL
  set(MKL_Link_Type Static)
  set(MKL_Interface_Layer 32)
  set(MKL_ThreadingLayer Sequential)
  set(MKL_OpenMP_Library iomp5)
  set(MKL_F95Interface BLAS95 LAPACK95)
  
  find_package(MKL REQUIRED)
  if(MKL_FOUND)
    set(bcls_HAVE_INTEL_MKL 1) # This needs to be defined for the C Preprocessor macro
  else()
    message(FATAL_ERROR "Use of Intel Math Kernel Library (MKL) has been requested but not found.")
  endif()

endif()

#------------------------------------------------------------------------------
# Find the system BLAS/LAPACK functions
# 
if (NOT BCLS_USE_MKL)
  if(APPLE)
    find_library(LAPACK_LIB Accelerate)
    include_directories("-FvecLib")
  else()
    message(STATUS "Linux users should install CBLAS from their package manager")
  endif()
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/src/ ${PROJECT_BINARY_DIR}/bcls)




