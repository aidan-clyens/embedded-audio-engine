add_library(framework INTERFACE)

target_sources(framework
  PUBLIC
  FILE_SET HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
      include/messagequeue.h
      include/observer.h
      include/subject.h
      include/threadedengine.h
)

target_include_directories(framework
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(framework PROPERTIES LINKER_LANGUAGE CXX)