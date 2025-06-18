#!/usr/bin/env python3
import rtmidi
import time
import sys

def midi_callback(message_data, data=None):
    """
    Callback function that is called for every incoming MIDI message.
    
    Parameters:
    - message_data: A tuple where the first element is a list of integers (MIDI message bytes)
                    and the second element is the timestamp.
    - data: Optional user data (not used in this sample).
    """
    message, timestamp = message_data
    print(f"Received MIDI message: {message} at {timestamp:.6f} seconds")

def main():
    # Create an instance of MidiIn
    midi_in = rtmidi.MidiIn()

    # Get available MIDI input ports
    available_ports = midi_in.get_ports()
    if available_ports:
        print("Available MIDI input ports:")
        for index, port in enumerate(available_ports):
            print(f"  {index}: {port}")
        try:
            port_index = int(input("Enter the port number to open (default is 0): ") or 0)
            # Open the first available port (adjust if needed)
            midi_in.open_port(port_index)
            print(f"Opened port {port_index}: {available_ports[port_index]}")
        except Exception as e:
            print("Failed to open MIDI port:", e)
            sys.exit(1)
    else:
        print("No MIDI input ports found. Opening a virtual MIDI input.")
        midi_in.open_virtual_port("Virtual MIDI Input")

    # Set callback to handle messages as they arrive
    midi_in.set_callback(midi_callback)

    print("Listening for MIDI input... Press Ctrl+C to exit.")
    
    try:
        # Keep running to listen for MIDI messages.
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        midi_in.close_port()

if __name__ == '__main__':
    main()

