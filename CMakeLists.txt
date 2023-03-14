cmake_minimum_required(VERSION 3.12)
project(main VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(libs/Utils)
add_subdirectory(libs/Window)
add_subdirectory(libs/Types)
add_subdirectory(libs/VerletObject)
add_subdirectory(libs/VerletRect)
add_subdirectory(libs/VerletCircle)
add_subdirectory(libs/VerletSolver)


SET (deps_inc "/opt/homebrew/include/")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${deps_inc}
    ${PROJECT_SOURCE_DIR}
    "."
)


find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)

link_libraries(main PRIVATE "/opt/homebrew/lib/")
target_link_libraries(main glfw OpenGL::GL GLEW::GLEW)

target_include_directories(main PRIVATE libs/Constants)
target_include_directories(main PRIVATE libs/Utils)
target_include_directories(main PRIVATE libs/Window)
target_include_directories(main PRIVATE libs/Types)
target_include_directories(main PRIVATE libs/VerletObject)
target_include_directories(main PRIVATE libs/VerletRect)
target_include_directories(main PRIVATE libs/VerletCircle)
target_include_directories(main PRIVATE libs/VerletSolver)

target_link_libraries(
    main 
    Utils 
    Window 
    Types
    VerletObject
    VerletCircle
    VerletRect
    VerletSolver
)

target_link_libraries(main "-framework Cocoa")
target_link_libraries(main "-framework OpenGL")
target_link_libraries(main "-framework IOKit")
