#ifndef _MIDI_ENGINE_H
#define _MIDI_ENGINE_H

#include <memory>
#include <vector>

#include "miditypes.h"
#include "subject.h"
#include "threadedengine.h"

class RtMidiIn;  // Forward declaration for RtMidiIn class

namespace Midi
{

/** @class MidiEngine
 *  @brief The MidiEngine class is responsible for managing MIDI input.
 */
class MidiEngine : public ThreadedEngine<MidiMessage>, public Subject<MidiMessage>
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

  void push_message(const MidiMessage& message)
  {
    m_message_queue.push(message);
  }

private:
  MidiEngine();
  ~MidiEngine() override;

  void run() override;

  void process_message(const MidiMessage& message);

  std::unique_ptr<RtMidiIn> p_midi_in;
};

}  // namespace Midi

#endif  // _MIDI_ENGINE_H
