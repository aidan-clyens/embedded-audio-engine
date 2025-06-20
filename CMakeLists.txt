cmake_minimum_required(VERSION 3.25)

project(DAW LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(thirdparty/rtmidi)
add_subdirectory(src/audioengine)
add_subdirectory(src/midiengine)
add_subdirectory(src/trackmanager)
add_subdirectory(src)
