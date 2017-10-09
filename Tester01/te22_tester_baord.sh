#!/bin/sh

init() {
    lsof  |grep snd |xargs -n 1 kill -9             &> /dev/null
    lsof  |grep ttyS1 |xargs -n 1 kill -9           &> /dev/null
    killall -9 miio_wifi                            &> /dev/null
    killall -9 pvalg_ymhood                         &> /dev/null
}





cat /dev/ttyS1

