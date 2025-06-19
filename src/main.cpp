#include <iostream>
#include "audioengine.h"
#include "midiengine.h"

int main() {
    std::cout << "Hello, Digital Audio Workstation!" << std::endl;
    
    Midi::MidiEngine midiEngine;

    return 0;
}
