#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <RtMidi.h>
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Midi
{

enum class eMidiMessageType : unsigned char
{
  NoteOff                   = 0x80,
  NoteOn                    = 0x90,
  PolyphonicKeyPressure     = 0xA0,
  ControlChange             = 0xB0,
  ProgramChange             = 0xC0,
  ChannelPressure           = 0xD0,
  PitchBendChange           = 0xE0,
  SystemExclusive           = 0xF0,
  MidiTimeCodeQuarterFrame  = 0xF1,
  SongPositionPointer       = 0xF2,
  SongSelect                = 0xF3,
  TuneRequest               = 0xF6,
  EndOfSysEx                = 0xF7,
  TimingClock               = 0xF8,
  Start                     = 0xFA,
  Continue                  = 0xFB,
  Stop                      = 0xFC,
  ActiveSensing             = 0xFE,
  SystemReset               = 0xFF
};

const std::map<eMidiMessageType, std::string> midi_message_type_names =
{
  {eMidiMessageType::NoteOff, "Note Off"},
  {eMidiMessageType::NoteOn, "Note On"},
  {eMidiMessageType::PolyphonicKeyPressure, "Polyphonic Key Pressure"},
  {eMidiMessageType::ControlChange, "Control Change"},
  {eMidiMessageType::ProgramChange, "Program Change"},
  {eMidiMessageType::ChannelPressure, "Channel Pressure"},
  {eMidiMessageType::PitchBendChange, "Pitch Bend Change"},
  {eMidiMessageType::SystemExclusive, "System Exclusive"},
  {eMidiMessageType::MidiTimeCodeQuarterFrame, "MIDI Time Code Quarter Frame"},
  {eMidiMessageType::SongPositionPointer, "Song Position Pointer"},
  {eMidiMessageType::SongSelect, "Song Select"},
  {eMidiMessageType::TuneRequest, "Tune Request"},
  {eMidiMessageType::EndOfSysEx, "End of SysEx"},
  {eMidiMessageType::TimingClock, "Timing Clock"},
  {eMidiMessageType::Start, "Start"},
  {eMidiMessageType::Continue, "Continue"},
  {eMidiMessageType::Stop, "Stop"},
  {eMidiMessageType::ActiveSensing, "Active Sensing"},
  {eMidiMessageType::SystemReset, "System Reset"}
};

/** @struct MidiPort
 *  @brief Represents a MIDI port with its number and name.
 */
struct MidiPort
{
  unsigned int port_number;
  std::string port_name;
};

/** @struct MidiMessage
 *  @brief Represents a MIDI message with its delta time received, status, data bytes, and type name.
 */
struct MidiMessage
{
  double deltatime;  // Time in seconds since the last message
  eMidiMessageType status;  // Status byte of the MIDI message
  unsigned char data1;   // First data byte (e.g., note number, control change number)
  unsigned char data2;   // Second data byte (e.g., velocity, control change value)
  std::string type_name; // Human-readable name of the MIDI message type
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
  void close_input_port();

private:
  std::unique_ptr<RtMidiIn> p_midi_in;
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
