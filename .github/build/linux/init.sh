#!/bin/sh
sudo apt update && sudo apt install gcc-multilib g++-multilib -y
git clone https://github.com/thecybermind/qmm2.git ../qmm2
git clone https://github.com/thecybermind/qmm_sdks.git ../qmm_sdks
