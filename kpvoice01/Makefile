#common makefile header


DIR_INC1:=include ssuperStream01 base01
DIR_INC9:=$(foreach aa1,$(DIR_INC1),-I$(aa1))
DIR_BIN = bin/

TARGET	= kpServer01.i386.bin
BIN_TARGET = $(DIR_BIN)/$(TARGET)

KDIR1:=/usr/src/kernels/$(shell uname -r) /usr/src/kernels/*
KDIR2:=$(firstword $(wildcard $(KDIR1)))

ifeq (,$(KDIR2))
$(error ' kernel source dir $(KDIR1) empty : $(KDIR2)' )
endif

all : clean $(BIN_TARGET) 

CROSS_COMPILE = 
CFLAGS = -g -Wall -Werror $(DIR_INC9) -I$(KDIR2) -I.

LDFLAGS += -lc -lstdc++

SRCs1:=$(wildcard \
	base01/*.c base01/*.cpp \
	ssuperStream01/*.c ssuperStream01/*.cpp \
	src/*.c src/*.cpp \
	kp01/*.c kp01/*.cpp )
OBJECTS := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRCs1)))

$(BIN_TARGET) : $(OBJECTS)
	[ -d bin/ ] || mkdir -p bin/
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)
	-chmod -R a+w bin/
	strip $@
	ls -l bin/*

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@

%.o : %.cpp
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@

c clean:
	@rm -f *.o $(BIN_TARGET) $(OBJECTS)

t test :
	bin/kpServer01.i386.bin

.PHONY:clean

#common makefile foot
