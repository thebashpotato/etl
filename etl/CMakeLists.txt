#
# NOTE: Set our source and include directory variables for use through the build
#
set(APP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(APP_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(APP_EXAMPLES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

#
# NOTE: Prevent in source builds (can't build in src/ or in project root)
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR OR CMAKE_CURRENT_BINARY_DIR STREQUAL APP_INCLUDE_DIR)
  message(
    FATAL_ERROR
      "In source builds are not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

#
# NOTE: If dev mode is enabled
#
if(ETL_DEV_MODE)
  # Enable examples
  message(STATUS "${PROJECT_NAME} -- Examples Enabled")
  add_subdirectory("examples")

  # Add tests
  message(STATUS "${PROJECT_NAME} -- Tests Enabled")
  add_subdirectory("tests")
endif()

#
# NOTE: Output build param information
#
message(STATUS "${PROJECT_NAME}-${PROJECT_VERSION} [${CMAKE_BUILD_TYPE}]")

#
# NOTE: Target as header only library
#
add_library(${ETL_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${ETL_TARGET_NAME} ALIAS ${ETL_TARGET_NAME})
target_compile_features(${ETL_TARGET_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${APP_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

#
# NOTE: Package/Configure and Installation.
# Only execute this code if we are not in developer mode.
#
if(NOT ETL_DEV_MODE)
  include(CMakePackageConfigHelpers)

  # Configure pkg-config file
  configure_file(${ETL_PKG_CONFIG_TEMPLATE} ${ETL_PKG_CONFIG_FILE} @ONLY)

  # Configure Cmake project config file
  configure_file(${ETL_CMAKE_PROJECT_CONFIG_TEMPLATE} ${ETL_CMAKE_PROJECT_CONFIG_FILE} @ONLY)

  # Configure Cmake version file
  configure_file(${ETL_CMAKE_VERSION_CONFIG_TEMPLATE} ${ETL_CMAKE_VERSION_CONFIG_FILE} @ONLY)

  # Install the header file
  install(DIRECTORY "${APP_INCLUDE_DIR}/" DESTINATION ${ETL_INCLUDE_INSTALL_DIR})

  # Install the pkg-config file
  install(FILES ${ETL_PKG_CONFIG_FILE} DESTINATION ${ETL_PKG_CONFIG_INSTALL_DIR})

  # Install the Cmake config files
  install(FILES ${ETL_CMAKE_VERSION_CONFIG_FILE} ${ETL_CMAKE_PROJECT_CONFIG_FILE}
          DESTINATION ${ETL_CMAKE_CONFIG_INSTALL_DIR})
endif()
