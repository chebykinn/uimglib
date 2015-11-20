CFLAGS=-ansi -pedantic -Wall -Werror -g -lm
CC=gcc
OBJECTS=image.o main.o
TARGET=uimglib

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)