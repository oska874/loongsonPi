#!/bin/sh

echo 37 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio37/direction
echo 1 > /sys/class/gpio/gpio37/value
echo 0 > /sys/class/gpio/gpio37/value
echo 37 > /sys/class/gpio/unexport
