cmake_minimum_required(VERSION 3.10)
project(Spider)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp" "includes/*.h")

add_executable(Spider ${SOURCES})

#OpenGL libs linker
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

