# Use the latest stable Ubuntu image
FROM ubuntu:latest

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Update and install required packages
RUN apt-get update && \
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
        alsa-utils \
    && rm -rf /var/lib/apt/lists/*

# Install project dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        librtmidi-dev \
        librtaudio-dev \
    && rm -rf /var/lib/apt/lists/*

# Configure ALSA to use the null device for audio simulation
RUN echo 'pcm.!default {\n    type null\n}' > /root/.asoundrc

# Set working directory
WORKDIR /workspace/DAW

# Default command
CMD ["/bin/bash"]