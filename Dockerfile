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
    && rm -rf /var/lib/apt/lists/*

# Install project dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        librtmidi-dev \
        librtaudio-dev
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]