#ifndef __AUDIO_SOURCE_H_
#define __AUDIO_SOURCE_H_

#include <memory>
#include <vector>

namespace Audio
{

class IAudioSource
{
public:
  IAudioSource(): audio_data_buffer(std::make_shared<std::vector<float>>()) {}
  virtual ~IAudioSource() {}

  std::shared_ptr<std::vector<float>> get_next_audiO_frame() const 
  {
    return audio_data_buffer;
  }

private:
  std::shared_ptr<std::vector<float>> audio_data_buffer;
};

}  // namespace Audio

#endif  // __AUDIO_SOURCE_H_
