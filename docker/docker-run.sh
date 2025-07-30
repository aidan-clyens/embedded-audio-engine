docker run -it --rm --init --platform=linux/arm64 \
    --name arm64-audio \
    -v "$(pwd):/home/$(whoami)/DAW" \
    -v "/home/$(whoami)/.ssh:/root/.ssh" \
    -v "/tmp/.X11-unix:/tmp/.X11-unix" \
    -w "/home/$(whoami)/DAW" \
    --device /dev/snd \
    arm64-audio:latest "$@"
