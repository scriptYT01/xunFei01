
.PHONY : t01

DSTs:= t01 

all: help01
help01:
	@echo ; echo $(DSTs)
	@echo

m:
	vim Makefile

t01 : t01.bin
	./$< 

t01.bin : t01.cpp
	g++ -o $@  $<

