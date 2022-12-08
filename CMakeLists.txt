cmake_minimum_required(VERSION 3.23)
project(fuzzycs)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR C:/dev/SFML-2.5.1-mingw32-7.3.0/lib/cmake/SFML)
set(SOURCE_FILES
        main.cpp)

include_directories(
        C:/dev/SFML-2.5.1-mingw32-7.3.0/include)
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(fuzzycs ${SOURCE_FILES})
target_link_libraries(fuzzycs sfml-graphics sfml-window sfml-system opengl32)