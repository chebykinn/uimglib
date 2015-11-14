FLAGS=-ansi -pedantic -Wall -Werror -g -lm
CC=gcc
OBJECTS=image.o main.o
TARGET=uimglib

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@

main.o:
	$(CC) $(FLAGS) -c main.c

image.o:
	$(CC) $(FLAGS) -c image.c

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)