#
# NOTE: Set our source and include directory variables for use through the build
#
set(APP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(APP_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(APP_EXAMPLES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

#
# NOTE: Prevent in source builds (can't build in src/ or in project root)
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR OR CMAKE_CURRENT_BINARY_DIR STREQUAL APP_SOURCE_DIR)
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
# NOTE: Configures cmake config and version files
#
configure_package_config_file(EtlConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EtlConfig.cmake
                              INSTALL_DESTINATION ${LIB_INSTALL_DIR}/Etl/cmake)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/EtlConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

#
# NOTE: Target as header only library
#
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE>:${APP_INCLUDE_DIR}>
                                                     $<INSTALL_INTERFACE:include>)

#
# NOTE: Installs the header file and the configuration files
#
install(DIRECTORY "${APP_INCLUDE_DIR}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EtlConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/EtlConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/Etl/cmake)
