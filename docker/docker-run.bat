docker run -it --rm ^
    -v %cd%:/workspace/DAW ^
    -v %USERPROFILE%\.ssh:/root/.ssh ^
    --network host ^
    --name raspi-daw ^
    raspi-daw