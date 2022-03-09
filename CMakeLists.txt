cmake_minimum_required(VERSION 3.14)
project(A8)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/mingw/include")
include_directories("C:/mingw64/include")
include_directories("C:/mingw64/x86_64-w64-mingw32/include")

add_executable(A8 main.cpp Bubble.h Bubble.cpp)

target_link_directories(A8 PUBLIC "C:/mingw/lib")
target_link_directories(A8 PUBLIC "C:/mingw64/lib")
target_link_directories(A8 PUBLIC "C:/mingw64/x86_64-w64-mingw32/lib")

target_link_libraries(A8 sfml-graphics sfml-system sfml-window sfml-main)
