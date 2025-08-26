#include "audioengine.h"
#include "midiengine.h"
#include "trackmanager.h"
#include "track.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <vector>

static bool app_running = false;

/** @brief Signal handler for graceful shutdown on SIGINT (Ctrl+C).
 *  This function sets the app_running flag to false, allowing the main loop to exit cleanly.
 *
 *  @param signum The signal number (not used here).
 */
void signal_handler(int signum) 
{
  std::cout << "\nExiting.\n";
  app_running = false;
}

/** @brief Opens a MIDI input device.
 *  This function lists available MIDI ports, prompts the user to select one,
 *  and attempts to open the selected MIDI input port.
 *  @return True if the MIDI device was opened successfully, false otherwise.
 */
bool open_midi_device()
{
  Midi::MidiEngine& midi_engine = Midi::MidiEngine::instance();

  // Return a list of supported MIDI devices
  std::vector<Midi::MidiPort> ports = midi_engine.get_ports();

  std::cout << "---------------------" << std::endl;
  for (const auto& port : ports)
  {
    std::cout << port.port_number << " - " << port.port_name << std::endl;
  }

  unsigned int selected_port;
  std::cout << "Enter a port number to use: ";
  std::cin >> selected_port;

  // Open selected MIDI device
  try
  {
    midi_engine.open_input_port(selected_port);
    std::cout << "Opened MIDI input port: " << selected_port << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error opening MIDI input port: " << e.what() << std::endl;
    return false;
  }

  return true;
}

/** @brief Main function for the Digital Audio Workstation application.
 *  @return Exit status of the application (0 for success, non-zero for failure).
 */
int main()
{
  Audio::AudioEngine& audio_engine = Audio::AudioEngine::instance();
  Midi::MidiEngine& midi_engine = Midi::MidiEngine::instance();
  Tracks::TrackManager& track_manager = Tracks::TrackManager::instance();
  
  std::cout << "Embedded Audio Engine" << std::endl;
  std::cout << "---------------------" << std::endl;

  std::cout << "Select audio or MIDI input device..." << std::endl;
  std::cout << "1 - Audio Input" << std::endl;
  std::cout << "2 - MIDI Input" << std::endl;
  std::cout << "3 - Sample File" << std::endl;
  std::cout << "Enter your choice (1-3): ";
  int choice;
  std::cin >> choice;

  std::cout << std::endl;

  if (choice == 1)
  {
    std::cout << "Using audio input device." << std::endl;
  }
  else if (choice == 2)
  {
    std::cout << "Using MIDI input device." << std::endl;
    open_midi_device();
  }
  else if (choice == 3)
  {
    std::cout << "Using sample file input." << std::endl;
  }
  else
  {
    std::cerr << "Invalid choice. Exiting." << std::endl;
    return 1;
  }

  std::signal(SIGINT, signal_handler);
  app_running = true;
  
  audio_engine.start_thread();
  midi_engine.start_thread();

  size_t track_index = track_manager.add_track();
  auto track = track_manager.get_track(track_index);
  
  // Attach track as observer to both engines
  midi_engine.attach(track);

  while (app_running)
  {
    track->handle_midi_message();

    // Wait for the signal handler to set app_running to false
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  midi_engine.detach(track);
  midi_engine.stop_thread();

  track_manager.clear_tracks();

  audio_engine.stop();

  return 0;
}
