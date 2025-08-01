#!/bin/bash

# ./kitware-archive.sh || { echo "Failed to run kitware-archive.sh"; exit 1; }

apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    gdb \
    git \
    vim \
    wget \
    curl \
    ca-certificates \
    openssh-client \
    alsa-utils

# Install project dependencies
apt-get install -y --no-install-recommends \
    librtmidi-dev \
    librtaudio-dev \
    portaudio19-dev \
    libgtest-dev

# Install python and pip
apt-get install -y --no-install-recommends \
    python3 \
    python3-dev \
    python3-pip \
    python3-venv
