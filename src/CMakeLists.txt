add_subdirectory(framework)
add_subdirectory(audioengine)
add_subdirectory(midiengine)
add_subdirectory(trackmanager)

add_executable(DAW
  main.cpp
)

target_link_libraries(DAW PRIVATE
  audioengine
  midiengine
  trackmanager
)
