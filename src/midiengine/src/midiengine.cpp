#include "midiengine.h"
#include "alsa_utils.h"

#include <rtmidi/RtMidi.h>
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace Midi;

/** @brief Callback function to handle incoming MIDI messages.
 *  This function is called by the RtMidi library when a MIDI message is received.
 *  It prints the received MIDI message to the console.
 *
 *  @param deltatime The time in seconds since the last message was received.
 *  @param message A vector containing the MIDI message bytes.
 *  @param user_data A pointer to the MidiEngine object.
 */
void midi_callback(double deltatime, std::vector<unsigned char> *message, void *user_data)
{
  assert(message != nullptr && "Received null MIDI message");
  assert(user_data != nullptr && "User data is null in MIDI callback");

  MidiEngine *midi_engine = static_cast<MidiEngine *>(user_data);
  assert(midi_engine != nullptr && "MidiEngine instance is null in MIDI callback");

  // Parse incoming MIDI messages
  MidiMessage midi_message;

  midi_message.deltatime = deltatime;
  midi_message.status = message->at(0);
  midi_message.type = static_cast<eMidiMessageType>(midi_message.status & 0xF0);
  midi_message.channel = midi_message.status & 0x0F;
  midi_message.data1 = message->size() > 1 ? message->at(1) : 0;
  midi_message.data2 = message->size() > 2 ? message->at(2) : 0;

  auto it = midi_message_type_names.find(midi_message.type);
  midi_message.type_name = it != midi_message_type_names.end() ? it->second : "Unknown MIDI Message";

  midi_engine->push_message(midi_message);
}

/** @brief Constructor for the MidiEngine class.
 */
MidiEngine::MidiEngine()
{
  if (!is_alsa_seq_available())
  {
    std::cout << "ALSA sequencer not available, skipping MIDI input initialization." << std::endl;
    p_midi_in = nullptr;
    return;
  }

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
  close_input_port();
}

/** @brief Lists all available MIDI input ports.
 *  This function retrieves and prints the names of all available MIDI input ports.
 *
 *  @return A vector of MidiPort objects representing the available MIDI ports.
 */
std::vector<MidiPort> MidiEngine::get_ports()
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  std::vector<MidiPort> ports;

  // Get the number of available MIDI input ports
  unsigned int port_count = p_midi_in->getPortCount();
  std::cout << "Number of MIDI input ports: " << port_count << std::endl;

  // List all available MIDI input ports
  for (unsigned int i = 0; i < port_count; ++i)
  {
    try
    {
      std::string port_name = p_midi_in->getPortName(i);
      ports.push_back({i, port_name});
    } catch (const RtMidiError &error)
    {
      std::cerr << "Error getting port name: " << error.getMessage() << std::endl;
    }
  }

  return ports;
}

/** @brief Opens a MIDI input port.
 *  @param port_number The index of the MIDI port to open (default is 0).
 *  @throws std::out_of_range if the port_number is invalid.
 *  @throws std::runtime_error if the port cannot be opened.
 */
void MidiEngine::open_input_port(unsigned int port_number)
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  if (port_number >= p_midi_in->getPortCount())
  {
    throw std::out_of_range("Invalid MIDI port number: " + std::to_string(port_number));
  }

  // Set up the MIDI input port
  try
  {
    p_midi_in->openPort(port_number);
  } catch (const RtMidiError &error)
  {
    throw std::runtime_error("Failed to open MIDI input port: " + error.getMessage());
  }

  std::cout << "MIDI input port opened successfully." << std::endl;
  // Set the callback function to handle incoming MIDI messages
  p_midi_in->setCallback(&midi_callback, this);
  p_midi_in->ignoreTypes(false, true, true);
}

/** @brief Closes the currently opened MIDI input port.
 */
void MidiEngine::close_input_port()
{
  assert(p_midi_in != nullptr && "MIDI input instance is not initialized");

  if (!p_midi_in->isPortOpen())
  {
    std::cout << "No MIDI input port is currently open." << std::endl;
    return;
  }

  try
  {
    p_midi_in->closePort();
    std::cout << "MIDI input port closed successfully." << std::endl;
  } catch (const RtMidiError &error)
  {
    std::cerr << "Error closing MIDI input port: " << error.getMessage() << std::endl;
  }
}

/** @brief The main loop of the MidiEngine thread.
 *  This function continuously checks for incoming MIDI messages and processes them.
 */
void MidiEngine::run() {
  while (m_running)
  {
    MidiMessage message = m_message_queue.pop();
    // Process the MIDI message here, or call a user-defined handler
    std::cout << "[Thread] Received MIDI message: " << message << std::endl;
    process_message(message);
  }
}

/** @brief Process a MIDI message.
 *  This function is called to handle a MIDI message after it has been dequeued.
 *  It can be overridden by subclasses to implement custom processing logic.
 *
 *  @param message The MIDI message to process.
 */
void MidiEngine::process_message(const MidiMessage& message)
{
  notify(message);
}
