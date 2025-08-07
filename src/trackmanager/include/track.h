#ifndef __TRACK_H__
#define __TRACK_H__

#include <queue>
#include <mutex>
#include <memory>

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

  // Observer interface
  void update(const Midi::MidiMessage& message) override;
  void update(const Audio::AudioMessage& message) override;

  void handle_midi_message();

  void get_next_audio_frame(float *output_buffer, unsigned int n_frames);

private:
  std::queue<Midi::MidiMessage> m_message_queue;
  std::mutex m_queue_mutex;
};

}  // namespace Tracks

#endif  // __TRACK_H__