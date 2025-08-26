#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "audioengine.h"
#include "trackmanager.h"
#include "filesystem.h"
#include "devicemanager.h"
#include "logger.h"

using namespace Audio;
using namespace Tracks;
using namespace Files;
using namespace Devices;

TEST(TrackManagerIntegrationTest, SingleTrack)
{
  set_thread_name("Main");

  // Start the audio engine
  AudioEngine::instance().start_thread();

  // Add a track
  ASSERT_EQ(TrackManager::instance().get_track_count(), 0);
  size_t track_index = TrackManager::instance().add_track();
  ASSERT_EQ(TrackManager::instance().get_track_count(), 1);

  auto track = TrackManager::instance().get_track(track_index);

  LOG_INFO("Track added with index: ", track_index);

  AudioEngineStatistics stats = AudioEngine::instance().get_statistics();
  LOG_INFO("Tracks playing: ", stats.tracks_playing);
  LOG_INFO("Total frames processed: ", stats.total_frames_processed);

  // Open a test WAV file and load it into the track
  std::string test_wav_file = "samples/test.wav";

  WavFile wav_file = FileSystem::instance().read_wav_file(test_wav_file);
  ASSERT_EQ(wav_file.get_filepath(), FileSystem::instance().convert_to_absolute(test_wav_file));
  ASSERT_EQ(wav_file.get_filename(), FileSystem::instance().convert_to_absolute(test_wav_file).filename().string());

  LOG_INFO("WAV file loaded: ", wav_file.get_filepath());

  track->add_wav_file_input(wav_file);

  // Add audio output to track
  track->add_audio_output();

  track->play();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Read audio engine statistics after adding the track and WAV file
  stats = AudioEngine::instance().get_statistics();
  LOG_INFO("Tracks playing: ", stats.tracks_playing);
  LOG_INFO("Total frames processed: ", stats.total_frames_processed);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  track->stop();

  // Wait until AudioEngine stops streaming
  while (AudioEngine::instance().get_state() != eAudioEngineState::Idle)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  // Read audio engine statistics after stopping
  stats = AudioEngine::instance().get_statistics();
  LOG_INFO("Tracks playing: ", stats.tracks_playing);
  LOG_INFO("Total frames processed: ", stats.total_frames_processed);

  // Stop the audio engine
  AudioEngine::instance().stop_thread();
}
