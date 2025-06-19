#include "audioengine.h"
#include "midiengine.h"

#include <iostream>

int main() {
    std::cout << "Hello, Digital Audio Workstation!" << std::endl;
    
    Midi::MidiEngine midi_engine;

    midi_engine.get_ports();

    return 0;
}
