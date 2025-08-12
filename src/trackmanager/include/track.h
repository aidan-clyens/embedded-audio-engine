#ifndef __TRACK_H__
#define __TRACK_H__

#include <queue>
#include <mutex>
#include <memory>
#include <optional>

#include "observer.h"
#include "midiengine.h"
#include "audiosource.h"

// Forward declaration
namespace Audio {
  struct AudioMessage;
}

namespace Tracks
{

/** @class Track
 *  @brief The Track class represents a track in the Digital Audio Workstation.
 */
class Track : public Observer<Midi::MidiMessage>, 
          public Observer<Audio::AudioMessage>,
          public Audio::IAudioSource, 
          public std::enable_shared_from_this<Track>
{
public:
  Track() = default;

  void add_audio_input(const unsigned int device_index = 0);
  void add_midi_input() {}
  void add_audio_output(const unsigned int device_index = 0);

  bool has_audio_input() const { return m_audio_input_device_index.has_value(); }
  bool has_midi_input() const { return false; }
  bool has_audio_output() const { return m_audio_output_device_index.has_value(); }

  unsigned int get_audio_input() const { return m_audio_input_device_index.value_or(std::numeric_limits<unsigned int>::max()); }
  unsigned int get_audio_output() const { return m_audio_output_device_index.value_or(std::numeric_limits<unsigned int>::max()); }

  // Observer interface
  void update(const Midi::MidiMessage& message) override;
  void update(const Audio::AudioMessage& message) override;

  void handle_midi_message();

  void get_next_audio_frame(float *output_buffer, unsigned int n_frames);

private:
  std::queue<Midi::MidiMessage> m_message_queue;
  std::mutex m_queue_mutex;

  std::optional<unsigned int> m_audio_input_device_index;
  std::optional<unsigned int> m_audio_output_device_index;
};

}  // namespace Tracks

#endif  // __TRACK_H__