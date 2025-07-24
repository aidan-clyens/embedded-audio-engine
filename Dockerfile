# Use the latest stable Ubuntu image
FROM arm64v8/ubuntu

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Update and install required packages
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
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
RUN apt-get install -y --no-install-recommends \
    librtmidi-dev \
    librtaudio-dev

# Configure ALSA to use the null device for audio simulation
RUN echo 'pcm.!default {\n    type null\n}' > /root/.asoundrc

# Clean up apt cache
RUN rm -rf /var/lib/apt/lists/*

# Default command
CMD ["/bin/bash"]