EXEC = blink-webcam

CFLAGS   = -W -Wall -O2 -pipe -g -ansi -pedantic -std=gnu99
LDFLAGS  = -lv4l2

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -fv *.o

mrproper: clean
	rm -fv $(EXEC)
