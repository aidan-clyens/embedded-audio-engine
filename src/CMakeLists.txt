add_executable(DAW
  main.cpp
)

target_link_libraries(DAW PRIVATE
  audioengine
  midiengine
  trackmanager
)
