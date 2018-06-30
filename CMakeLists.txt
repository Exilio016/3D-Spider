cmake_minimum_required(VERSION 3.10)
project(Spider)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} includes)

add_executable(Spider src/main.cpp src/spider.cpp includes/spider.h includes/types.h)

#OpenGL libs linker
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${OpenCV_LIBS})
