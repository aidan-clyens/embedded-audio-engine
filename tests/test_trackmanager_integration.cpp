#include <gtest/gtest.h>
#include <iostream>

#include "audioengine.h"
#include "trackmanager.h"

using namespace Audio;
using namespace Tracks;

TEST(TrackManagerIntegrationTest, SingleTrack)
{
  AudioEngine &audio_engine = AudioEngine::instance();
  TrackManager &track_manager = TrackManager::instance();

  // Start the audio engine and track manager
  audio_engine.start();
  track_manager.start();

  // Add a track
  ASSERT_EQ(track_manager.get_track_count(), 0);
  size_t track_index = track_manager.add_track();
  ASSERT_EQ(track_manager.get_track_count(), 1);

  auto track = track_manager.get_track(track_index);

  std::cout << "Track added with index: " << track_index << std::endl;

  AudioEngineStatistics stats = audio_engine.get_statistics();
  std::cout << "Tracks playing: " << stats.tracks_playing << std::endl;
  std::cout << "Total frames processed: " << stats.total_frames_processed << std::endl;

  // Open a test WAV file and load it into the track
  std::string test_wav_file = "test_audio.wav"; // Ensure this file exists

  // Stop the audio engine and track manager
  audio_engine.stop();
  track_manager.stop();
}
