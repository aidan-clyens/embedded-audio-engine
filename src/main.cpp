#include "audioengine.h"
#include "midiengine.h"

#include <iostream>

int main() {
  std::cout << "Hello, Digital Audio Workstation!" << std::endl;
  
  Midi::MidiEngine midi_engine;

  std::vector<Midi::MidiPort> ports = midi_engine.get_ports();

  for (const auto& port : ports) {
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

  return 0;
}
