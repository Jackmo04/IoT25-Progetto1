#!/bin/bash
mkdir build 2> /dev/null; arduino-cli compile -b arduino:avr:uno --output-dir ./build .