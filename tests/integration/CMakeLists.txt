enable_testing()

add_executable(EmbeddedAudioEngineTests
  test_trackmanager_integration.cpp
)

target_link_libraries(EmbeddedAudioEngineTests PRIVATE
  gtest
  gtest_main
  framework
  audioengine
  midiengine
  trackmanager
  filesystem
  devicemanager
)

add_test(NAME EmbeddedAudioEngineUnitTests COMMAND EmbeddedAudioEngineUnitTests)
add_test(NAME EmbeddedAudioEngineTests COMMAND EmbeddedAudioEngineTests)