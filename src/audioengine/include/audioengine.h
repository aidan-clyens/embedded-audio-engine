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

  void open_input_file(const std::string& filename, unsigned int audio_device = 0);

private:
  AudioEngine();
  ~AudioEngine() override;

  void run() override;

  std::unique_ptr<RtAudio> p_audio_in;
};

}  // namespace Audio

#endif  // _AUDIO_ENGINE_H
