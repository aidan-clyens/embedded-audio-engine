#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <RtMidi.h>

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

private:
  RtMidiIn *p_midi_in;
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
