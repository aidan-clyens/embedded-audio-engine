#include "devicemanager.h"
#include "audioengine.h"
#include "midiengine.h"

using namespace Devices;

std::vector<AudioDevice> DeviceManager::get_audio_devices() const
{
  std::vector<AudioDevice> devices;
  auto audio_devices = Audio::AudioEngine::instance().get_devices();

  size_t index = 0;
  for (const auto& info : audio_devices)
  {
    AudioDevice device;
    device.id = index++;
    device.name = info.name;
    device.input_channels = info.inputChannels;
    device.output_channels = info.outputChannels;
    device.duplex_channels = info.duplexChannels;
    device.is_default_input = info.isDefaultInput;
    device.is_default_output = info.isDefaultOutput;
    device.sample_rates = info.sampleRates;
    device.preferred_sample_rate = info.preferredSampleRate;
    devices.push_back(device);
  }

  return devices;
}

AudioDevice DeviceManager::get_audio_device(const unsigned int id) const
{
  std::vector<AudioDevice> devices = get_audio_devices();
  for (const auto &device : devices)
  {
    if (device.id == id)
    {
      return device;
    }
  }


  throw std::runtime_error("Audio device with ID " + std::to_string(id) + " does not exist");
}