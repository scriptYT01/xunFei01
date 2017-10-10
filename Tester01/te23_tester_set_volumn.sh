#!/bin/sh

amixer sset TITANIUM  ${1}%                     &> /dev/null 
amixer sset MERCURY   ${1}%                     &> /dev/null 

