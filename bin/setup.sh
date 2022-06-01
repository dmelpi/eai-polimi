#!/bin/bash
# Setup script for hsdatalog under Linux
# Must be run as root

cp libhs_datalog.so /usr/lib

cp 30-hsdatalog.rules /etc/udev/rules.d

#create hsdatalog group
echo "Adding hsdatalog group"
addgroup hsdatalog

#TODO check if the group already exists
echo "Adding user to hsdatalog group"
usermod -aG hsdatalog $USER

udevadm control --reload-rules
udevadm trigger

