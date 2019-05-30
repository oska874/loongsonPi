#!/bin/sh

echo "init gpio "
echo 38> /sys/class/gpio/export
echo out> /sys/class/gpio/gpio38/direction
for((i=1;i<=10;i++));
do
    echo "light on"
    echo 1 > /sys/class/gpio/gpio38/value
    sleep 1
    echo "light off"
    echo 0 > /sys/class/gpio/gpio38/value
    sleep 1
done
echo 38> /sys/class/gpio/unexport

