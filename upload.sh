#!/bin/bash

FILE=$1

# ./build/ftp_client udp://127.0.0.1:14569 1 put $FILE /fs/microsd
./build/ftp_client udp://192.168.1.35:14550 1 put $FILE /fs/microsd
