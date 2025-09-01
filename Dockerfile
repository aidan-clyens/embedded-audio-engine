# Use the latest stable Ubuntu image
FROM arm64v8/ubuntu:22.04

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

COPY kitware-archive.sh kitware-archive.sh
COPY configure.sh configure.sh
RUN ./kitware-archive.sh || { echo "Failed to run kitware-archive.sh"; exit 1; }
RUN ./configure.sh

# Configure Python environment
COPY requirements.txt /opt/requirements.txt

RUN python3 -m venv /opt/venv
RUN /opt/venv/bin/pip install --upgrade pip
RUN /opt/venv/bin/pip install -r /opt/requirements.txt

# Configure ALSA to use the null device for audio simulation
RUN echo 'pcm.!default {\n    type null\n}' > /root/.asoundrc

# Clean up apt cache
RUN rm -rf /var/lib/apt/lists/*

# Default command
COPY docker/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]


CMD ["/bin/bash"]
