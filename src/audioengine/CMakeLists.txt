add_library(audioengine STATIC
  include/audioengine.h
  src/audioengine.cpp
)

target_sources(audioengine
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES include/audioengine.h
)

target_sources(audioengine PRIVATE src/audioengine.cpp)
