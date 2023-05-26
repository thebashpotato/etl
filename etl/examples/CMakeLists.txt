#
# NOTE: add the executables
#
set(ScratchFile "${PROJECT_NAME}-scratch")
set(Blackjack "${PROJECT_NAME}-blackjack")

add_executable(${ScratchFile} "${APP_EXAMPLES_SOURCE_DIR}/scratch.cpp" ${UTILS_SOURCE_FILES})
add_executable(${Blackjack} "${APP_EXAMPLES_SOURCE_DIR}/blackjack.cpp" ${UTILS_SOURCE_FILES})

target_include_directories(${ScratchFile} PUBLIC ${APP_INCLUDE_DIR})
target_include_directories(${Blackjack} PUBLIC ${APP_INCLUDE_DIR})

target_link_libraries(${ScratchFile} PRIVATE project_options project_warnings)
target_link_libraries(${Blackjack} PRIVATE project_options project_warnings)
