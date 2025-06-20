#include "track.h"

#include <iostream>

using namespace Tracks;

/** @brief Updates the track with a new MIDI message.
 *  This function is called by the MidiEngine when a new MIDI message is received.
 *  @param message The MIDI message to process.
 */
void Track::update(const Midi::MidiMessage& message)
{
  std::lock_guard<std::mutex> lock(m_queue_mutex);
  m_message_queue.push(message);
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