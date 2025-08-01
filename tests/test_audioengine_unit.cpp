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

/** @brief Get available audio devices.
 *  This test checks if the audio engine can retrieve a list of available audio devices.
 */
TEST(AudioEngineTest, GetAudioDevices)
{
  Audio::AudioEngine& engine = Audio::AudioEngine::instance();

  std::vector<RtAudio::DeviceInfo> devices = engine.get_devices();
  std::cout << "Available Audio Devices:" << std::endl;
  for (const auto& device : devices)
  {
      std::cout << "Device Name: " << device.name << ", Input Channels: " << device.inputChannels
              << ", Output Channels: " << device.outputChannels << std::endl;
  }

  ASSERT_FALSE(devices.empty()) << "No audio devices found";
}