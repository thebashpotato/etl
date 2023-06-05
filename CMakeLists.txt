cmake_minimum_required(VERSION 3.18)

#
# NOTE: Set workspace name, and language.
#
project(
  etl
  VERSION 0.2.0
  DESCRIPTION "C++ Extra Template Library"
  HOMEPAGE_URL "https://github.com/thebashpotato/etl"
  LANGUAGES CXX)

#
# NOTE: Set the C++ standard
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#
# NOTE: add additional project options
#
option(ETL_DEV_MODE "Enables testing and example builds" OFF)

#
# NOTE: Add our custom cmake modules to the cmake module path
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#
# NOTE: Include custom cmake dev-scripts, and the CPM package manager and add
# compiler warnings from our projects cmake/ directory.
#
include(CPM)
include(StandardProjectSettings)
include(StaticAnalyzers)
include(CompilerWarnings)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#
# NOTE: Creates interfaced "libraries" for
# The custom StandardProjectSettings and CompilerWarnings.
# These libraries can then be linked to the tests, examples and
# release executables to ensure the correct c++ standard is used,
# and all defined warnings are used.
#
add_library(etl_project_options INTERFACE)
target_compile_features(etl_project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(etl_project_warnings INTERFACE)
set_project_warnings(etl_project_warnings)

#
# NOTE: FindPackage and CPM (Dependencies needed by all projects are defined at
# this level only.
#

if(ETL_DEV_MODE)
  # Enable examples
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Forcing build type to debug" FORCE)

  # NOTE: Bring in Gtest Unit testing library
  cpmaddpackage(
    NAME
    googletest
    GITHUB_REPOSITORY
    google/googletest
    GIT_TAG
    v1.13.0
    VERSION
    1.13.0
    OPTIONS
    "BUILD_GMOCK ON"
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON")
  if(googletest_ADDED)
    message(STATUS "gtest configured correctly")
  else()
    message(WARNING "gtest was not configured properly")
  endif()
  enable_testing()
else()
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Forcing build type to release" FORCE)
endif()

#
# NOTE: Add sub-projects to the project
#
add_subdirectory("etl")
