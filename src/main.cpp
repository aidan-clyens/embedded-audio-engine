#include "audioengine.h"
#include "midiengine.h"
#include "trackmanager.h"
#include "track.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <vector>

using namespace Audio;
using namespace Midi;
using namespace Tracks;

static bool app_running = false;

/** @class Application
 *  @brief The main Audio Engine Platform application
 */
class Application
{
public:
  Application()
  {
    AudioEngine::instance().start_thread();
    MidiEngine::instance().start_thread();
  }

  ~Application()
  {
    MidiEngine::instance().stop_thread();
    AudioEngine::instance().stop_thread();
  }

  void run()
  {
    size_t track_index = TrackManager::instance().add_track();
    auto track = TrackManager::instance().get_track(track_index);

    // Attach track as observer to both engines
    MidiEngine::instance().attach(track);

    while (app_running)
    {
      track->handle_midi_message();

      // Wait for the signal handler to set app_running to false
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    MidiEngine::instance().detach(track);

    TrackManager::instance().clear_tracks();
  }
};

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
  std::cout << "Embedded Audio Engine" << std::endl;
  std::cout << "---------------------" << std::endl;

  std::signal(SIGINT, signal_handler);
  app_running = true;

  Application app;
  app.run();

  return 0;
}
