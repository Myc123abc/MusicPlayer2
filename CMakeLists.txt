cmake_minimum_required(VERSION 3.10)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SOURCE_FILES "main.cpp" "MusicPlayer/*.cpp" "Music/*.cpp")
file(GLOB SHARED_LIBRARIES "bass/*.so")

add_executable(player ${SOURCE_FILES})
target_link_libraries(player PRIVATE ${SHARED_LIBRARIES} ncurses)

set_target_properties(player PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
