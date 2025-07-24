docker run -it --rm --platform=linux/arm64 \
    --name arm64-audio \
    -v "$(pwd):/home/$(whoami)/DAW" \
    -v "/home/$(whoami)/.ssh:/root/.ssh" \
    -v "/tmp/.X11-unix:/tmp/.X11-unix" \
    -w "/home/$(whoami)/DAW" \
    arm64-audio:latest "$@"
