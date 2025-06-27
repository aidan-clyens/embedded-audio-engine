#include "audioengine.h"
#include <stdexcept>
#include <thread>
#include <sndfile.h>
#include <iostream>

using namespace Audio;

/** @brief Called by the audio device to play audio data
 *  This function is called by the audio device to fill the output buffer with audio data.
 *  @param outputBuffer Pointer to the output buffer where audio data should be written
 *  @param inputBuffer Pointer to the input buffer (not used in this case)
 *  @param nFrames Number of frames to process
 *  @param streamTime Current stream time (not used in this case)
 *  @param status Stream status (not used in this case)
 *  @param userData Pointer to user data (in this case, a vector of audio data)
 *  @return 0 on success, non-zero on error
 */
int audio_callback(void* outputBuffer, void* inputBuffer, unsigned int nFrames,
                  double streamTime, RtAudioStreamStatus status, void* userData)
{
  std::vector<float> *audio_data = static_cast<std::vector<float>*>(userData);
  static size_t pos = 0;
  float *out = static_cast<float*>(outputBuffer);

  for (unsigned int i = 0; i < nFrames; ++i)
  {
    if (pos < audio_data->size())
    {
      out[i] = (*audio_data)[pos++];
    }
    else
    {
      out[i] = 0.0f; // Fill with silence if we run out of data
    }
  }

  return 0;
}

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

/** @brief Open an audio input file
 *  @param filename The path to the audio file to open
 *  @throws std::runtime_error if the file cannot be opened
 */
void AudioEngine::open_input_file(const std::string& filename, unsigned int audio_device)
{
  if (p_audio_in->isStreamOpen())
  {
    p_audio_in->closeStream();
  }

  // Open the audio file
  SF_INFO sfinfo;
  SNDFILE* infile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
  if (!infile)
  {
    throw std::runtime_error("Failed to open audio file " + filename + "(" + sf_strerror(nullptr) + ")");
  }

  std::vector<float> audio_data = std::vector<float>(sfinfo.frames * sfinfo.channels);
  sf_readf_float(infile, audio_data.data(), sfinfo.frames);
  sf_close(infile);

  // Open the audio stream
  RtAudio::StreamParameters parameters;
  parameters.deviceId = audio_device;
  parameters.nChannels = sfinfo.channels;

  unsigned int sample_rate = sfinfo.samplerate;
  unsigned int buffer_frames = 256;

  try
  {
    p_audio_in->openStream(nullptr, &parameters, RTAUDIO_FLOAT32, sample_rate, &buffer_frames, 
                           &audio_callback, &audio_data);
    p_audio_in->startStream();
  }
  catch (const RtAudioError& e)
  {
    throw std::runtime_error("Failed to open audio stream: " + std::string(e.getMessage()));
  }
}
