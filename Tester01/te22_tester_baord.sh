#!/bin/sh

initTTY() {
    lsof  |grep snd     |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null
    lsof  |grep ttyS1   |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null

    killall -9 miio_wifi                            &> /dev/null
    killall -9 pvalg_ymhood                         &> /dev/null
    stty -F /dev/ttyS1 -hupcl ignbrk -onlcr -echoe -echok -echoctl -echoke min 1 time 5
}



initTTY


while read -r line1 ; do
    #echo "Text read from file: <${line1}>"
    line2=$(echo -n ${line1}|tr -d '\n\r')
    wc1=$(echo -n ${line2}|wc -w)
    if [ 2 = "${wc1}" ]
    then
        echo "line2 <${line2}>"
    fi
done < /dev/ttyS1


