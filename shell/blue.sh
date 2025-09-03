#!/bin/bash
rfkill unblock bluetooth
hciattach -s 1500000 /dev/ttyS1 any 1500000 flow nosleep
hciconfig hci0 up
