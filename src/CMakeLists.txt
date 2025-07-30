add_subdirectory(framework)
add_subdirectory(audioengine)
add_subdirectory(midiengine)
add_subdirectory(trackmanager)

add_executable(EmbeddedAudioEngine
  main.cpp
)

target_link_libraries(EmbeddedAudioEngine PRIVATE
  audioengine
  midiengine
  trackmanager
)
