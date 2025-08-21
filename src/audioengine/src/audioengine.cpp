#include "audioengine.h"
#include "alsa_utils.h"

#include <cassert>
#include <stdexcept>
#include <thread>
#include <iostream>

using namespace Audio;

/** @brief AudioEngine constructor
 */
AudioEngine::AudioEngine() : ThreadedEngine("AudioEngine")
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

/** @brief Play command
 */
void AudioEngine::play()
{
  AudioMessage msg;
  msg.command = eAudioEngineCommand::Play;
  push_message(msg);
}

/** @brief Stop command
 */
void AudioEngine::stop()
{
  AudioMessage msg;
  msg.command = eAudioEngineCommand::Stop;
  push_message(msg);
}

/** @brief Run the audio engine
 */
void AudioEngine::run()
{
  while (is_running())
  {
    handle_messages();
    update_state();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void AudioEngine::handle_messages()
{
  while (auto message = try_pop_message())
  {
    switch (message->command)
    {
      case eAudioEngineCommand::Play:
        LOG_INFO("AudioEngine: Change state to Running");
        m_state = eAudioEngineState::Running;
        break;
      case eAudioEngineCommand::Stop:
        LOG_INFO("AudioEngine: Change state to Stopped");
        m_state = eAudioEngineState::Stopped;
        break;
      default:
        throw std::runtime_error("AudioEngine: Invalid command received");
        break;
    }
  }
}

/** @brief The Audio Engine application state machine
 */
void AudioEngine::update_state()
{
  switch (m_state)
  {
    case Init:
      break;
    case Idle:
      break;
    case Stopped:
      break;
    case Running:
      update_state_running();
      break;
    default:
      throw std::runtime_error("Unknown Audio Engine state");
  }
}

void AudioEngine::update_state_running()
{
  
}

/** @brief Process audio for the current tracks in the Track Manager
 *  @param output_buffer Pointer to the output audio buffer
 *  @param n_frames Number of frames to process
 */
void AudioEngine::process_audio(float *output_buffer, unsigned int n_frames)
{
  // Clear output buffer as tracks will accumulate into it
  std::fill(output_buffer, output_buffer + n_frames, 0.0f);

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
