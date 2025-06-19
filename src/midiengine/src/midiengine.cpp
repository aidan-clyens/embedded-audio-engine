#include "midiengine.h"

using namespace Midi;

/** @brief Constructor for the MidiEngine class.
 */
MidiEngine::MidiEngine()
{
  p_midi_in = new RtMidiIn();
  if (!p_midi_in)
  {
    throw std::runtime_error("Failed to create MIDI input instance");
  }

  // Set up the MIDI input port
  try
  {
    p_midi_in->openPort(0); // Open the first available MIDI input port
    std::cout << "MIDI input port opened successfully." << std::endl;
  } catch (const RtMidiError &error)
  {
    delete p_midi_in;
    p_midi_in = nullptr;
    throw std::runtime_error("Failed to open MIDI input port: " + error.getMessage());
  }
}

/** @brief Destructor for the MidiEngine class.
 */
MidiEngine::~MidiEngine()
{
  if (p_midi_in)
  {
    try
    {
      p_midi_in->closePort(); // Close the MIDI input port
    } catch (const RtMidiError &error)
    {
      // Handle error if necessary, but do not throw in destructor
    }

    delete p_midi_in;
  }

  p_midi_in = nullptr;
}
