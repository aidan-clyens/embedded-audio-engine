#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <RtMidi.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

#include "subject.h"

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
  unsigned char status;  // Status byte of the MIDI message
  eMidiMessageType type; // Type of the MIDI message (e.g., Note On, Control Change)
  unsigned char channel; // MIDI channel (0-15)
  unsigned char data1;   // First data byte (e.g., note number, control change number)
  unsigned char data2;   // Second data byte (e.g., velocity, control change value)
  std::string type_name; // Human-readable name of the MIDI message type
};

inline std::ostream& operator<<(std::ostream& os, const MidiMessage& msg)
{
  os << "MidiMessage { "
     << "deltatime: " << msg.deltatime
     << ", status: 0x" << std::hex << static_cast<int>(msg.status) << std::dec
     << ", type: " << msg.type_name
     << ", channel: " << static_cast<int>(msg.channel)
     << ", data1: " << static_cast<int>(msg.data1)
     << ", data2: " << static_cast<int>(msg.data2)
     << " }";
  return os;
}

/** @class MidiEngine
 *  @brief The MidiEngine class is responsible for managing MIDI input.
 */
class MidiEngine : public Subject<MidiMessage>
{
public:
  static MidiEngine& instance()
  {
    static MidiEngine instance;
    return instance;
  }

  std::vector<MidiPort> get_ports();

  void open_input_port(unsigned int port_number = 0);
  void close_input_port();

  void enqueue_message(const MidiMessage& message)
  {
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    m_message_queue.push(message);
  }

  MidiMessage dequeue_message()
  {
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    if (m_message_queue.empty())
    {
      throw std::runtime_error("No MIDI messages in the queue");
    }

    MidiMessage message = m_message_queue.front();
    m_message_queue.pop();
    return message;
  }

  // Thread control
  void start();
  void stop();
  bool is_running() const;

private:
  MidiEngine();
  virtual ~MidiEngine();
  MidiEngine(const MidiEngine&) = delete;
  MidiEngine& operator=(const MidiEngine&) = delete;

  void run(); // Thread main loop

  void process_message(const MidiMessage& message);
  void process_note_on(const MidiMessage& message);
  void process_note_off(const MidiMessage& message);
  void process_control_change(const MidiMessage& message);

  std::unique_ptr<RtMidiIn> p_midi_in;

  std::queue<MidiMessage> m_message_queue;
  std::mutex m_queue_mutex;

  std::thread m_thread;
  std::atomic<bool> m_running{false};
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
