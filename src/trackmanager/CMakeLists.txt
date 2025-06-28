add_library(trackmanager STATIC)

target_sources(trackmanager
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/../include
    FILES
      include/track.h
      ../include/threadedengine.h
      ../include/observer.h
)

target_sources(trackmanager
  PRIVATE
  src/trackmanager.cpp
  src/track.cpp
)

target_include_directories(trackmanager
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(trackmanager PUBLIC
  midiengine
  audioengine
)
