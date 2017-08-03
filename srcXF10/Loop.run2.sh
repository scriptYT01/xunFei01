#!/bin/sh

cd /ch/xf10

killall led_flash.sh

while [ 1 ]
do
    ./Loop.run1.sh
done
