#ifndef __AUDIO_DEVICE_H_
#define __AUDIO_DEVICE_H_

#include <rtaudio/RtAudio.h>


namespace Audio
{

class AudioDevice
{
public:
  AudioDevice() {}
  virtual ~AudioDevice() {}

private:
  RtAudio::DeviceInfo m_device_info;
  RtAudio::StreamOptions m_stream_options;
  RtAudio::StreamParameters m_stream_parameters;
};

}  // namespace Audio

#endif  // __AUDIO_DEVICE_H
