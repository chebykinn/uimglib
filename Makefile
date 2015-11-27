CFLAGS=-ansi -pedantic -Wall -Werror -g -fPIC
FLAGS=-shared
BINFLAGS=-ansi -pedantic -Wall -Werror -g -L. -luimg -lm
CC=gcc
OBJECTS=bmp24/bmp.o image.o
TARGET=uimglib
TARGET_LIB=libuimg.so
BINOBJECTS=main.o

all: $(TARGET)

$(TARGET): $(TARGET_LIB) $(BINOBJECTS)
	$(CC) $(BINFLAGS) $(BINOBJECTS) -o $@

$(TARGET_LIB): $(OBJECTS)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET_LIB)
	rm -f $(BINOBJECTS)
	rm -f $(TARGET)