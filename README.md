### MAVSDK Examples
Taken and adapted from the [MAVSDK examples](https://github.com/mavlink/MAVSDK/tree/main/examples/).

This repo exists to provide an easy and minimal way to download/build/install various mavsdk helper programs.

### Build
```
make
```

### Install (installs to ~/.local/bin)
```
make install 
```

### Run
For example to test an FTDI connection between Host PC and Telem1
```
./build/tester1/tester1 serial:///dev/ttyUSB0:57600
```
