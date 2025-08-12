#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "audioengine.h"
#include "trackmanager.h"
#include "filesystem.h"

using namespace Audio;
using namespace Tracks;
using namespace Files;

TEST(TrackManagerIntegrationTest, SingleTrack)
{
  AudioEngine &audio_engine = AudioEngine::instance();
  FileSystem &file_system = FileSystem::instance();
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

  // Add an audio input to the track
  track->add_audio_input();
  unsigned int input_device_index = audio_engine.get_default_input_device();
  ASSERT_EQ(track->get_audio_input(), input_device_index);

  // Open a test WAV file and load it into the track
  std::string test_wav_file = "samples/test.wav";

  WavFile wav_file = file_system.read_wav_file(test_wav_file);
  ASSERT_EQ(wav_file.get_filepath(), file_system.convert_to_absolute(test_wav_file));
  ASSERT_EQ(wav_file.get_filename(), file_system.convert_to_absolute(test_wav_file).filename().string());

  std::cout << "WAV file loaded: " << wav_file.get_filepath() << std::endl;

  track->add_wav_file_input(wav_file);

  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Read audio engine statistics after adding the track and WAV file
  stats = audio_engine.get_statistics();
  std::cout << "Tracks playing: " << stats.tracks_playing << std::endl;
  std::cout << "Total frames processed: " << stats.total_frames_processed << std::endl;

  // Stop the audio engine and track manager
  audio_engine.stop();
  track_manager.stop();
}
