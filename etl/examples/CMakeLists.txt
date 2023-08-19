#
# NOTE: add the executables
#
set(ScratchFile "${PROJECT_NAME}-scratch")
set(Blackjack "${PROJECT_NAME}-blackjack")
set(MoveOnly "${PROJECT_NAME}-moveonly")

add_executable(${ScratchFile} "${APP_EXAMPLES_SOURCE_DIR}/scratch.cpp" ${UTILS_SOURCE_FILES})
add_executable(${Blackjack} "${APP_EXAMPLES_SOURCE_DIR}/blackjack/main.cpp" ${UTILS_SOURCE_FILES})
add_executable(${MoveOnly} "${APP_EXAMPLES_SOURCE_DIR}/moveonly/main.cpp" ${UTILS_SOURCE_FILES})

target_include_directories(${ScratchFile} PUBLIC ${APP_INCLUDE_DIR})
target_include_directories(${Blackjack} PUBLIC ${APP_INCLUDE_DIR})
target_include_directories(${MoveOnly} PUBLIC ${APP_INCLUDE_DIR})

target_link_libraries(${ScratchFile} PRIVATE etl_project_options etl_project_warnings)
target_link_libraries(${Blackjack} PRIVATE etl_project_options etl_project_warnings)
target_link_libraries(${MoveOnly} PRIVATE etl_project_options etl_project_warnings)
