#!/bin/sh

initTTY() {
    lsof  |grep snd     |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null
    lsof  |grep ttyS1   |awk '{print $1}' |xargs -n 1 kill -9           &> /dev/null

    killall -9 miio_wifi                            &> /dev/null
    killall -9 pvalg_ymhood                         &> /dev/null
    stty -F /dev/ttyS1 -hupcl ignbrk -onlcr -echoe -echok -echoctl -echoke min 1 time 5
}

if [ "$1" = "stop" ]
then
    killall -9 pvalg_ymhood                         &> /dev/null
    killall $(basename $0)
    exit
fi


initTTY


hh1=/vt/VIOMI_test_wav/M2CHN02VM_AAQ0
hh2=.wav

if [ 1 = 1 ]
then
    while [ 1 ] ; do
        read -r line1 
        echo "Text read from file: <${line1}>"
    done < /dev/ttyS1
    exit
fi

while read -r line1 ; do
    #echo "Text read from file: <${line1}>"
    line2=$(echo -n ${line1}|tr -d '\n\r')
    wc1=$(echo -n ${line2}|wc -w)
    if [ 2 = "${wc1}" ]
    then
        back1="play failed1" 
        word1=$(echo -n ${line2}|awk '{print $1}')
        word2=$(echo -n ${line2}|awk '{print $2}')
        if [ 'play' = "${word1}" ]
        then
            wav1="${hh1}${word2}${hh2}"
            #echo "wav1 1: <${wav1}>"
            if [ -f "${wav1}" ]
            then
                echo "wav1 2: <${wav1}>"
                aplay -f S16_LE -r 16000 "${wav1}"
                back1="play ok1" 
            fi
            echo ${back1} "${wav1}" > /dev/ttyS1
        fi
    fi
done < /dev/ttyS1


