#!/bin/bash

./firmware.py --build-path=../../../../../litex-boards/litex_boards/targets/build/digilent_basys3_vul --mem=rom
#dd if=/dev/zero bs=1 seek=33554432 count=0 of=firmware.fbi
echo "Firmware Built"
