#!/bin/sh

init() {
    lsof  |grep snd |xargs -n 1 kill -9             &> /dev/null
    lsof  |grep ttyS1 |xargs -n 1 kill -9           &> /dev/null
    killall -9 miio_wifi                            &> /dev/null
    killall -9 pvalg_ymhood                         &> /dev/null
    stty -F /dev/ttyS1 -hupcl ignbrk -onlcr -echoe -echok -echoctl -echoke min 1 time 5
}





cat /dev/ttyS1

