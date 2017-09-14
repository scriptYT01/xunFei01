#common makefile header


DIR_INC = include
DIR_BIN = bin/

TARGET	= kpServer01.i386.bin
BIN_TARGET = $(DIR_BIN)/$(TARGET)

all : clean $(BIN_TARGET) 

CROSS_COMPILE = 
CFLAGS = -g -Wall -Werror -I$(DIR_INC) -I.

LDFLAGS += -lc -lstdc++

SRCs1:=$(wildcard src/*.c src/*.cpp kp01/*.c kp01/*.cpp )
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

.PHONY:clean

#common makefile foot
