add_library(audioengine STATIC
  include/audioengine.h
  src/audioengine.cpp
)

target_sources(audioengine
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS 
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/../include
    FILES
      include/audioengine.h
      include/audiodevice.h
      ../include/threadedengine.h

)

target_sources(audioengine PRIVATE src/audioengine.cpp)

target_include_directories(audioengine
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(audioengine PUBLIC
  rtaudio
  sndfile
  trackmanager
)
