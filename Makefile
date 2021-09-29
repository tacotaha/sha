CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -O3 #-g
OBJ=sha.o
EXEC=sha test

all: $(EXEC)

sha: main.o $(OBJ)
	$(CC) main.o $(OBJ) -o sha

test: test.o $(OBJ)
	$(CC) test.o $(OBJ) -lcrypto -lpthread -o test

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

sha.o: sha.h sha.c
	$(CC) $(CFLAGS) -c sha.c -o sha.o

clean:
	rm -f $(EXEC) *.o *~

format:
	find . -name "*.c" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
	find . -name "*.h" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
