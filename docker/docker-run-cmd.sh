docker run -it --rm --init --platform=linux/arm64 \
    --name embedded-audio-engine \
    -v "$(pwd):/home/$(whoami)/embedded-audio-engine" \
    -v "/home/$(whoami)/.ssh:/root/.ssh" \
    -v "/tmp/.X11-unix:/tmp/.X11-unix" \
    -w "/home/$(whoami)/embedded-audio-engine" \
    embedded-audio-engine:latest "$@"
