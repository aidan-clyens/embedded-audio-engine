enable_testing()

add_executable(EmbeddedAudioEngineTests
  test_audioengine_unit.cpp
  test_filesystem_unit.cpp
  test_trackmanager_integration.cpp
)

target_link_libraries(EmbeddedAudioEngineTests PRIVATE
  gtest
  gtest_main
  audioengine
  filesystem
)

add_test(NAME EmbeddedAudioEngineTests COMMAND EmbeddedAudioEngineTests)