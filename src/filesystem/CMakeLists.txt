add_library(filesystem STATIC)

target_sources(filesystem
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
      include/filesystem.h
)

target_sources(filesystem PRIVATE src/filesystem.cpp)

target_include_directories(filesystem
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(filesystem
  PUBLIC
    sndfile
)

set_target_properties(filesystem PROPERTIES LINKER_LANGUAGE CXX)