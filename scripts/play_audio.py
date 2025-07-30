#!/usr/bin/env python3
import pyaudio
import wave
import time

# Define the chunk size for reading the file in small pieces
CHUNK = 1024

def callback(in_data, frame_count, time_info, status):
    """
    This callback reads frames from the WAV file and returns them.
    It allows PyAudio to request more data as needed, which can improve timing.
    """
    data = wf.readframes(frame_count)
    if len(data) < frame_count * wf.getsampwidth():
        # End of file reached, signal termination
        return (data, pyaudio.paComplete)
    return (data, pyaudio.paContinue)

# Open the WAV file
wf = wave.open('test.wav', 'rb')

# Create a PyAudio instance
p = pyaudio.PyAudio()

# Open an output stream using parameters from the WAV file
stream = p.open(
    format=p.get_format_from_width(wf.getsampwidth()),
    channels=wf.getnchannels(),
    rate=wf.getframerate(),
    output=True,
    stream_callback=callback
)

# Start the stream
stream.start_stream()

while stream.is_active():
    # Keep the main thread alive while the stream is active
    time.sleep(0.1)

# Properly close the stream and terminate the PyAudio instance
stream.stop_stream()
stream.close()
p.terminate()

print("Playback finished.")
