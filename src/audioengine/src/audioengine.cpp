#include "audioengine.h"
#include "trackmanager.h"

#include <stdexcept>
#include <thread>
#include <sndfile.h>
#include <iostream>

using namespace Audio;

/** @brief AudioEngine constructor
 */
AudioEngine::AudioEngine()
{
  // Set up RtAudio
  p_audio_in = std::make_unique<RtAudio>();
  if (!p_audio_in)
  {
    throw std::runtime_error("Failed to create RtAudio instance");
  }
}

/** @brief AudioEngine destructor
 */
AudioEngine::~AudioEngine()
{
  
}

/** @brief Get a list of available audio devices
 *  @return A vector of available audio devices
 */
std::vector<RtAudio::DeviceInfo> AudioEngine::get_devices()
{
  std::vector<RtAudio::DeviceInfo> devices;
  for (unsigned int i = 0; i < p_audio_in->getDeviceCount(); i++)
  {
    RtAudio::DeviceInfo info = p_audio_in->getDeviceInfo(i);
    devices.push_back(info);
  }

  return devices;
}


/** @brief Run the audio engine
 */
void AudioEngine::run()
{
  while (m_running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

/** @brief Process audio for the current tracks in the Track Manager
 *  @param output_buffer Pointer to the output audio buffer
 *  @param n_frames Number of frames to process
 */
void AudioEngine::process_audio(float *output_buffer, unsigned int n_frames)
{
  Tracks::TrackManager &track_manager = Tracks::TrackManager::instance();
 
  std::vector<std::vector<float>> track_buffers(track_manager.get_track_count(), std::vector<float>(n_frames, 0.0f));
  for (size_t i = 0; i < track_manager.get_track_count(); ++i)
  {
    std::shared_ptr<Tracks::Track>track = track_manager.get_track(i);
    track->get_next_audio_frame(track_buffers[i].data(), n_frames);
  }

  // TODO - Send to Audio Mixer
}

/** @brief Audio callback function
 *  @param output_buffer Pointer to the output audio buffer
 *  @param input_buffer Pointer to the input audio buffer (not used here)
 *  @param n_frames Number of frames to process
 *  @param stream_time Current stream time
 *  @param status Stream status
 *  @param user_data User data pointer (should be AudioEngine instance)
 *  @return 0 on success, non-zero on error
 */
int AudioEngine::audio_callback(void *output_buffer, void *input_buffer, unsigned int n_frames,
                                 double stream_time, RtAudioStreamStatus status, void *user_data)
{
  AudioEngine *engine = static_cast<AudioEngine*>(user_data);
  if (!engine)
  {
    std::cerr << "AudioEngine pointer is null in callback" << std::endl;
    return 1; // Error code
  }

  engine->process_audio(static_cast<float*>(output_buffer), n_frames);
  return 0;
}
