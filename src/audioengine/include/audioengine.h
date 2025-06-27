#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#include <memory>
#include <string>
#include <rtaudio/RtAudio.h>

#include "threadedengine.h"

namespace Audio
{

struct AudioMessage
{

};

/** @class AudioEngine
 *  @brief 
 */
class AudioEngine : public ThreadedEngine<AudioMessage>
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

private:
  AudioEngine();
  ~AudioEngine() override;

  void process_audio(float *output_buffer, unsigned int n_frames);

  void run() override;

  static int audio_callback(void *output_buffer, void *input_buffer, unsigned int n_frames,
                     double stream_time, RtAudioStreamStatus status, void *user_data);

  std::unique_ptr<RtAudio> p_audio_in;
};

}  // namespace Audio

#endif  // _AUDIO_ENGINE_H
