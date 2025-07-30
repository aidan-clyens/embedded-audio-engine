# Embedded Audio Processing Engine for ARM64

## Platform

**Architecture:**<br>
x86_64<br>
ARM64<br>

**OS:**<br>
Ubuntu 22.04<br>
Raspberry PI OS<br>

## Getting Starting

**Clone Repo:**<br>
```bash
git clone ...AudioEngine.git
cd AudioEngine
```

### Configure for Linux x86_64
The following must be run as root

1. Add **KitWare** repository
    ```bash
    sudo ./kitware-archive.sh
    ```
2. Install build dependencies
    ```bash
    sudo ./configure.sh
    ```
3. Create a **Python** virtual environment and install required packages
    ```bash
    python3 -m venv venv
    source venv/bin/activate
    ```
    ```bash
    pip install --upgrade pip
    pip install -r requirements.txt
    ```

### Configure for Linux ARM64

### Building for Linux x86_64
1. Set up **CMake**
    ```bash
    cmake -S . -B build_x86_64
    ```
2. Build
    ```bash
    cmake --build build_x86_64
    ```

### Building for Linux ARM64

### Running for Linux x86_64
1. Make sure Audio devices are available
    ```bash
    python3 scripts/alsa_devices.py
    ```
2. Make sure audio output is working
    ```bash
    python3 scripts/play_audio.py
    ```



## Docs

## License