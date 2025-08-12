enable_testing()

add_executable(EmbeddedAudioEngineUnitTests
  test_audioengine_unit.cpp
  test_filesystem_unit.cpp
  test_trackmanager_unit.cpp
)

add_executable(EmbeddedAudioEngineTests
  test_trackmanager_integration.cpp
)

target_link_libraries(EmbeddedAudioEngineUnitTests PRIVATE
  gtest
  gtest_main
  audioengine
  trackmanager
  filesystem
)

target_link_libraries(EmbeddedAudioEngineTests PRIVATE
  gtest
  gtest_main
  audioengine
  trackmanager
  filesystem
)

add_test(NAME EmbeddedAudioEngineUnitTests COMMAND EmbeddedAudioEngineUnitTests)
add_test(NAME EmbeddedAudioEngineTests COMMAND EmbeddedAudioEngineTests)