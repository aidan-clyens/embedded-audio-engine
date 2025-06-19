#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <RtMidi.h>
#include <memory>

namespace Midi
{

/** @class MidiEngine
 *  @brief The MidiEngine class is responsible for managing MIDI input.
 */
class MidiEngine
{
public:   
  MidiEngine();
  virtual ~MidiEngine();

  void get_ports();

  bool open_input_port(unsigned int port_number = 0);

private:
  std::unique_ptr<RtMidiIn> p_midi_in;
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
