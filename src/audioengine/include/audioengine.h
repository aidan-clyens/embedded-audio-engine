#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <rtaudio/RtAudio.h>

#include "threadedengine.h"

namespace Devices
{
class DeviceManager;
}

namespace Audio
{

enum eAudioEngineState
{
  Idle,
  Init,
  Stopped,
  Running,
};

enum eAudioEngineCommand
{
  Play,
  Stop,
};

/** @struct AudioMessage
 *  @brief Audio Message structure used to comminicate within AudioEngine class.
 *  TO BE IMPLEMENTED
 */
struct AudioMessage
{
  eAudioEngineCommand command;
};

inline std::ostream& operator<<(std::ostream& os, const AudioMessage& message)
{
  return os << "AudioMessage";
}

/** @struct AudioEngineStatistics
 *  @brief Running statistics for the Audio Engine.
 */
struct AudioEngineStatistics
{
  unsigned int tracks_playing;
  unsigned int total_frames_processed;
};

/** @class AudioEngine
 *  @brief Handles internal audio processing.
 */
class AudioEngine : public ThreadedEngine<AudioMessage>
{
  friend class Devices::DeviceManager;

public:
  static AudioEngine& instance()
  {
    static AudioEngine instance;
    return instance;
  }

  AudioEngineStatistics get_statistics() const { return m_statistics; }

  void play();
  void stop();

private:
  AudioEngine();

  std::vector<RtAudio::DeviceInfo> get_devices();

  void process_audio(float *output_buffer, unsigned int n_frames);

  void run() override;
  void handle_messages() override;

  void update_state();
  void update_state_running();

  static int audio_callback(void *output_buffer, void *input_buffer, unsigned int n_frames,
                     double stream_time, RtAudioStreamStatus status, void *user_data);

  std::unique_ptr<RtAudio> p_audio_in;

  eAudioEngineState m_state;
  AudioEngineStatistics m_statistics;
};

}  // namespace Audio

#endif  // _AUDIO_ENGINE_H
