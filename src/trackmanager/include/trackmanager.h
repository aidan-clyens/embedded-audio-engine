#ifndef __TRACK_MANAGER_H_
#define __TRACK_MANAGER_H_

#include "threadedengine.h"
#include "midiengine.h"
#include "track.h"

#include <memory>
#include <vector>

namespace Tracks
{

/** @class TrackManager
 *  @brief The TrackManager class is responsible for managing tracks in the Digital Audio Workstation.
 *  It inherits from ThreadedEngine to handle MIDI messages and manage track operations.
 */
class TrackManager : public ThreadedEngine<Midi::MidiMessage>
{
public:
  static TrackManager& instance()
  {
    static TrackManager instance;
    return instance;
  }

  void add_track();
  void remove_track(size_t index);
  std::shared_ptr<Track> get_track(size_t index) const;

  void clear_tracks();

  size_t get_track_count() const { return m_tracks.size(); }

private:
  std::vector<std::shared_ptr<Track>> m_tracks;
};

}  // namespace Tracks

#endif  // __TRACK_MANAGER_H_