add_library(midiengine STATIC
  include/midiengine.h
  src/midiengine.cpp
)

target_sources(midiengine
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES include/midiengine.h
)

target_sources(midiengine PRIVATE src/midiengine.cpp)
