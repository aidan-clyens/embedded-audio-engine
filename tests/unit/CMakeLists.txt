enable_testing()

add_executable(EmbeddedAudioEngineUnitTests
  test_audioengine_unit.cpp
  test_filesystem_unit.cpp
  test_trackmanager_unit.cpp
  test_track_unit.cpp
)

target_link_libraries(EmbeddedAudioEngineUnitTests PRIVATE
  gtest
  gtest_main
  audioengine
  trackmanager
  filesystem
)

add_test(NAME EmbeddedAudioEngineUnitTests COMMAND EmbeddedAudioEngineUnitTests)