#include <gtest/gtest.h>
#include <iostream>

#include "trackmanager.h"
#include "track.h"
#include "audioengine.h"

using namespace Tracks;

/** @brief Track - Add Audio Input
 */
TEST(TrackTest, AddAudioInput)
{
  TrackManager::instance().clear_tracks();

  // Create a new track
  size_t index = TrackManager::instance().add_track();
  auto track = TrackManager::instance().get_track(index);

  // Add audio input to the track
  track->add_audio_input();

  // Verify the track has an audio input
  EXPECT_TRUE(track->has_audio_input());

  unsigned int expected_input_device_index = Audio::AudioEngine::instance().get_default_input_device();
  EXPECT_EQ(track->get_audio_input(), expected_input_device_index);
}

/** @brief Track - Add MIDI Input
 */
TEST(TrackTest, AddMidiInput)
{
  EXPECT_TRUE(false) << "This test is not implemented yet.";
}

/** @brief Track - Add Audio Output
 */
TEST(TrackTest, AddAudioOutput)
{
  TrackManager::instance().clear_tracks();

  // Create a new track
  size_t index = TrackManager::instance().add_track();
  auto track = TrackManager::instance().get_track(index);

  // Add audio output to the track
  track->add_audio_output();

  // Verify the track has an audio output
  EXPECT_TRUE(track->has_audio_output());
 
  unsigned int expected_output_device_index = Audio::AudioEngine::instance().get_default_output_device();
  EXPECT_EQ(track->get_audio_output(), expected_output_device_index);
}
