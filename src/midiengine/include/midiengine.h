#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <RtMidi.h>
#include <memory>
#include <vector>
#include <string>

namespace Midi
{

/** @struct MidiPort
 *  @brief Represents a MIDI port with its number and name.
 */
struct MidiPort
{
  unsigned int port_number;
  std::string port_name;
};
  
/** @class MidiEngine
 *  @brief The MidiEngine class is responsible for managing MIDI input.
 */
class MidiEngine
{
public:   
  MidiEngine();
  virtual ~MidiEngine();

  std::vector<MidiPort> get_ports();

  void open_input_port(unsigned int port_number = 0);

private:
  std::unique_ptr<RtMidiIn> p_midi_in;
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
