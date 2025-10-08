#include <gtest/gtest.h>
#include <iostream>

#include "midiengine.h"
#include "devicemanager.h"
#include "trackmanager.h"
#include "logger.h"

using namespace Midi;
using namespace Tracks;

TEST(MidiInputIntegrationTest, MidiInput)
{
  set_thread_name("Main");

  // Start the MIDI engine
  MidiEngine::instance().start_thread();

  auto midi_ports = MidiEngine::instance().get_ports();
  ASSERT_GE(midi_ports.size(), 0);
  for (auto &port : midi_ports)
  {
    LOG_INFO("MIDI Port: ", port.port_number, " - ", port.port_name);
  }

  // Create track
  size_t track_id = TrackManager::instance().add_track();
  auto track = TrackManager::instance().get_track(track_id);

  // Add MIDI input to track
  track->add_midi_input();

  // Stop the MIDI engine
  MidiEngine::instance().stop_thread();
}