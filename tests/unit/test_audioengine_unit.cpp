#include <gtest/gtest.h>
#include <iostream>

#include "audioengine.h"

/** @brief Run the audio engine.
 *  This test starts the audio engine and stops it.
 */
TEST(AudioEngineTest, RunAudioEngine)
{
  Audio::AudioEngine& engine = Audio::AudioEngine::instance();

  engine.start();
  // TODO - Add assertions to verify the engine is running properly.
  engine.stop();
  // TODO - Add assertions to verify the engine has stopped properly.
}
