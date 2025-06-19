#include "midiengine.h"

#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace Midi;

/** @brief Constructor for the MidiEngine class.
 */
MidiEngine::MidiEngine()
{
  p_midi_in = std::make_unique<RtMidiIn>();
  if (!p_midi_in)
  {
    throw std::runtime_error("Failed to create MIDI input instance");
  }
}

/** @brief Destructor for the MidiEngine class.
 */
MidiEngine::~MidiEngine()
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  try
  {
    p_midi_in->closePort(); // Close the MIDI input port
  } catch (const RtMidiError &error)
  {
    // Handle error if necessary, but do not throw in destructor
  }
}

/** @brief Lists all available MIDI input ports.
 *  This function retrieves and prints the names of all available MIDI input ports.
 */
void MidiEngine::get_ports()
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  // Get the number of available MIDI input ports
  unsigned int port_count = p_midi_in->getPortCount();
  std::cout << "Number of MIDI input ports: " << port_count << std::endl;

  // List all available MIDI input ports
  for (unsigned int i = 0; i < port_count; ++i)
  {
    try
    {
      std::string port_name = p_midi_in->getPortName(i);
      std::cout << "Port " << i << ": " << port_name << std::endl;
    } catch (const RtMidiError &error)
    {
      std::cerr << "Error getting port name: " << error.getMessage() << std::endl;
    }
  }
}

/** @brief Opens a MIDI input port.
 *  @param port_number The index of the MIDI port to open (default is 0).
 *  @return True if the port was opened successfully, false otherwise.
 */
bool MidiEngine::open_input_port(unsigned int port_number)
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  if (port_number >= p_midi_in->getPortCount())
  {
    throw std::out_of_range("Invalid MIDI port number: " + std::to_string(port_number));
  }

  // Set up the MIDI input port
  try
  {
    p_midi_in->openPort(0); // Open the first available MIDI input port
    std::cout << "MIDI input port opened successfully." << std::endl;
  } catch (const RtMidiError &error)
  {
    throw std::runtime_error("Failed to open MIDI input port: " + error.getMessage());
  }

  return true;
}
