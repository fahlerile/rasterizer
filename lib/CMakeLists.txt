add_subdirectory(c-codebase)

target_link_libraries(
    ${EXECUTABLE_NAME}
    Vector
    memoryUtils
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    SDL2 SDL2main
)

target_include_directories(
    ${EXECUTABLE_NAME} PUBLIC
    c-codebase
    include
)

