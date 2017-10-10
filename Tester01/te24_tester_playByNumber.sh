#!/bin/sh



hh1=/vt/VIOMI_test_wav/M2CHN02VM_AAQ0
hh2=.wav

wav1="${hh1}${1}${hh2}"
aplay -f S16_LE -r 16000 "${wav1}"
