#include "audioengine.h"
#include "alsa_utils.h"

#include <cassert>
#include <stdexcept>
#include <thread>
#include <iostream>

using namespace Audio;

/** @brief AudioEngine constructor
 */
AudioEngine::AudioEngine():
  ResourceEngine("AudioEngine")
{
  if (!is_alsa_seq_available())
  {
    LOG_INFO("ALSA sequencer not available, skipping audio input initialization.");
    p_audio_in = nullptr;
    return;
  }

  // Set up RtAudio
  p_audio_in = std::make_unique<RtAudio>();
  if (!p_audio_in)
  {
    throw std::runtime_error("Failed to create RtAudio instance");
  }
}

/** @brief Get a list of available audio devices
 *  @return A vector of available audio devices
 */
std::vector<RtAudio::DeviceInfo> AudioEngine::get_devices()
{
  assert(p_audio_in != nullptr && "Audio input instance is not initialized");

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
    handle_messages();
    update_state();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

/** @brief The Audio Engine application state machine
 */
void AudioEngine::update_state()
{
  switch (m_state)
  {
    case AUDIO_ENGINE_STATE_INIT:
      break;
    case AUDIO_ENGINE_STATE_IDLE:
      break;
    case AUDIO_ENGINE_STATE_STOPPED:
      break;
    case AUDIO_ENGINE_STATE_RUNNING:
      update_state_running();
      break;
    default:
      throw std::runtime_error("Unknown Audio Engine state");
  }
}

/** @brief Process audio for the current tracks in the Track Manager
 *  @param output_buffer Pointer to the output audio buffer
 *  @param n_frames Number of frames to process
 */
void AudioEngine::process_audio(float *output_buffer, unsigned int n_frames)
{
  // Clear output buffer as tracks will accumulate into it
  std::fill(output_buffer, output_buffer + n_frames, 0.0f);

  // Create audio message to notify observers (tracks)
  AudioMessage message;
  // Notify all observers (tracks) to process their audio
  notify(message);

  // Update statistics
  m_statistics.total_frames_processed += n_frames;
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
