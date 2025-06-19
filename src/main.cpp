#include "audioengine.h"
#include "midiengine.h"

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

/** @brief Main function for the Digital Audio Workstation application.
 *  @return Exit status of the application (0 for success, non-zero for failure).
 */
int main()
{
  std::cout << "Hello, Digital Audio Workstation!" << std::endl;
  
  Midi::MidiEngine& midi_engine = Midi::MidiEngine::instance();

  std::vector<Midi::MidiPort> ports = midi_engine.get_ports();

  for (const auto& port : ports)
  {
    std::cout << "Port Number: " << port.port_number 
              << ", Port Name: " << port.port_name << std::endl;
  }

  unsigned int selected_port;
  std::cout << "Enter a port number to use: ";
  std::cin >> selected_port;

  try
  {
    midi_engine.open_input_port(selected_port);
    std::cout << "Opened MIDI input port: " << selected_port << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error opening MIDI input port: " << e.what() << std::endl;
    return 1;
  }

  std::signal(SIGINT, signal_handler);
  app_running = true;

  while (app_running)
  {
    // Main loop can be extended to handle MIDI messages or other tasks
    try
    {
      Midi::MidiMessage message = midi_engine.dequeue_message();
      std::cout << "Received MIDI message: "
                << "Delta Time: " << message.deltatime
                << ", Status: " << static_cast<int>(message.status)
                << ", Type: " << message.type_name
                << ", Channel: " << static_cast<int>(message.channel)
                << ", Data1: " << static_cast<int>(message.data1)
                << ", Data2: " << static_cast<int>(message.data2) 
                << std::endl;
    }
    catch (const std::exception& e) {
      // Ignore
    }

    // Wait for the signal handler to set app_running to false
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
