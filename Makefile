CROSS_COMPILE ?= 
CC := $(CROSS_COMPILE)gcc
STRIP := $(CROSS_COMPILE)strip
CFLAGS += -g -O2 -Wall -I../libconf -L../libconf
LIBS = -lconf -static
OBJ := daemon

all: popen.o main.o
	$(CC) $(CFLAGS) -o $(OBJ) $^ $(LIBS)
	$(STRIP) $(OBJ)
	-chmod a+x $(OBJ)
.c.o:
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean:
	rm -rf *.o
	rm $(OBJ)
