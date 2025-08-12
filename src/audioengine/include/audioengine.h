#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <rtaudio/RtAudio.h>

#include "resourceengine.h"

namespace Audio
{

enum eAudioEngineState
{
  AUDIO_ENGINE_STATE_IDLE,
  AUDIO_ENGINE_STATE_INIT,
  AUDIO_ENGINE_STATE_STOPPED,
  AUDIO_ENGINE_STATE_RUNNING,
};

struct AudioMessage
{

};

inline std::ostream& operator<<(std::ostream& os, const AudioMessage& message)
{
  return os << "AudioMessage";
}

struct AudioEngineStatistics
{
  unsigned int tracks_playing;
  unsigned int total_frames_processed;
};

/** @class AudioEngine
 *  @brief 
 */
class AudioEngine : public ResourceEngine<AudioMessage>
{
public:
  static AudioEngine& instance()
  {
    static AudioEngine instance;
    return instance;
  }

  std::vector<RtAudio::DeviceInfo> get_devices();
  unsigned int get_default_input_device() const { return p_audio_in->getDefaultInputDevice(); }
  unsigned int get_default_output_device() const { return p_audio_in->getDefaultOutputDevice(); }

  AudioEngineStatistics get_statistics() const { return m_statistics; }

  void play() { m_state = AUDIO_ENGINE_STATE_RUNNING; }
  void stop() { m_state = AUDIO_ENGINE_STATE_STOPPED; }

private:
  AudioEngine();
  ~AudioEngine() override;

  void process_audio(float *output_buffer, unsigned int n_frames);

  void run() override;
  void update_state();

  void update_state_running() {}

  static int audio_callback(void *output_buffer, void *input_buffer, unsigned int n_frames,
                     double stream_time, RtAudioStreamStatus status, void *user_data);

  std::unique_ptr<RtAudio> p_audio_in;

  eAudioEngineState m_state;
  AudioEngineStatistics m_statistics;
};

}  // namespace Audio

#endif  // _AUDIO_ENGINE_H
