#!/bin/sh

initTTY() {
    lsof  |grep snd     |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null
    lsof  |grep ttyS1   |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null

    killall -9 miio_wifi                            &> /dev/null
    killall -9 pvalg_ymhood                         &> /dev/null
    stty -F /dev/ttyS1 -hupcl ignbrk -onlcr -echoe -echok -echoctl -echoke min 1 time 5

    touch /tmp/voice_enable
}

if [ "$1" = "stop" ]
then
    killall -9 pvalg_ymhood                         &> /dev/null
    killall $(basename $0)
    exit
fi

#if [ "$1" != "child" ]
#then
#
#    initTTY
#
#    /usr/bin/pvalg_ymhood 2 >& 1 | $0 child
#    exit
#
#fi
#
#
## cat /dev/ttyS1
#
#while [ 1 ]
#do
#    read -r line1
#    line2=$(echo -n ${line1}|tr -d '\n\r')
#    wc1=$(echo -n ${line2}|wc -w)
#    echo "wc1 ${wc1} <${line2}>"
#done

initTTY

#/usr/bin/pvalg_ymhood | \
#while [ 1 ]
#do
#    sleep 1
#    #read -r line1
#    #line2=$(echo -n ${line1}|tr -d '\n\r')
#    #wc1=$(echo -n ${line2}|wc -w)
#    #echo "wc1 ${wc1} <${line2}>"
#done

#/usr/bin/pvalg_ymhood 

while [ 1 ]
do
    sleep 1
    read -r line1
    line2=$(echo -n ${line1}|tr -d '\n\r')
    wc1=$(echo -n ${line2}|wc -w)
    echo "wc1 ${wc1} <${line2}>"
done < `/usr/bin/pvalg_ymhood`

