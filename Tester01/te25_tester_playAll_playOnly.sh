#!/bin/sh



hh1=/vt/VIOMI_test_wav/M2CHN02VM_AAQ0
hh2=.wav

#wav1="${hh1}${1}${hh2}"
#aplay -f S16_LE -r 16000 "${wav1}"

if [ -z "$1" ]
then
    GAP=5
else
    GAP=$1
fi

echo
echo

for aa1 in "${hh1}"*"${hh2}" 
do
    echo "aplay -f S16_LE -r 16000 ${aa1}"
          aplay -f S16_LE -r 16000 ${aa1} 
    echo

    sleep ${GAP}
done
