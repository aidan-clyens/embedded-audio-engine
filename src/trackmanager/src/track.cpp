#include "track.h"

#include "audioengine.h"

#include <iostream>
#include <stdexcept>

using namespace Tracks;

/** @brief Adds an audio input to the track.
 *  @param device_index The index of the audio input device. Defaults to 0 (the default input device).
 */
void Track::add_audio_input(const unsigned int device_index)
{
  // Audio::AudioEngine::instance().get_default_input_device();

  // Get audio input device
  auto devices = Audio::AudioEngine::instance().get_devices();
  if (devices.empty())
  {
    throw std::runtime_error("No audio input devices available.");
  }
  if (device_index >= devices.size())
  {
    throw std::out_of_range("Audio device index is out of range.");
  }

  RtAudio::DeviceInfo input_device = devices[device_index];

  // Verify the audio device has input channels
  if (input_device.inputChannels < 1)
  {
    throw std::runtime_error("Selected audio device " + input_device.name + " has no input channels.");
  }

  m_audio_input_device_index = device_index;
  std::cout << "Track: Added audio input device: " << input_device.name << std::endl;
}

/** @brief Updates the track with a new MIDI message.
 *  This function is called by the MidiEngine when a new MIDI message is received.
 *  @param message The MIDI message to process.
 */
void Track::update(const Midi::MidiMessage& message)
{
  std::lock_guard<std::mutex> lock(m_queue_mutex);
  m_message_queue.push(message);
}

/** @brief Updates the track with a new audio message.
 *  This function is called by the AudioEngine when a new audio message is received.
 *  @param message The audio message to process.
 */
void Track::update(const Audio::AudioMessage &message)
{
  (void)message;
}

/** @brief Handles a MIDI message.
 *  This function processes the MIDI message received from the MidiEngine.
 *  @param message The MIDI message to handle.
 */
void Track::handle_midi_message()
{
  Midi::MidiMessage message;
  {
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    if (m_message_queue.empty())
      return;
  
    message = m_message_queue.front();
    m_message_queue.pop();
  }

  // Process the MIDI message here
  switch (message.type)
  {
    case Midi::eMidiMessageType::NoteOn:
      std::cout << "Track: Note On - Channel: " << static_cast<int>(message.channel)
                << ", Note: " << static_cast<int>(message.data1)
                << ", Velocity: " << static_cast<int>(message.data2) << std::endl;
      break;
    case Midi::eMidiMessageType::NoteOff:
      std::cout << "Track: Note Off - Channel: " << static_cast<int>(message.channel)
                << ", Note: " << static_cast<int>(message.data1) << std::endl;
      break;
    case Midi::eMidiMessageType::ControlChange:
      std::cout << "Track: Control Change - Channel: " << static_cast<int>(message.channel)
                << ", Controller: " << static_cast<int>(message.data1)
                << ", Value: " << static_cast<int>(message.data2) << std::endl;
      break;
    default:
      std::cout << "Track: Unknown MIDI Message Type - " << message.type_name << std::endl;
      break;
  }
}

/** @brief Fill the audio output buffer with the next available data
 *  @param output_buffer Pointer to the output buffer where audio data will be written.
 *  @param n_frames Number of frames to fill in the output buffer.
 */
void Track::get_next_audio_frame(float *output_buffer, unsigned int n_frames)
{
  
}
