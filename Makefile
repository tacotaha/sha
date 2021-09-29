CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -O3 -march=native -mtune=native
OBJ=rcons.o sha.o sha256.o
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

rcons.o: rcons.h rcons.c
	$(CC) $(CFLAGS) -c rcons.c -o rcons.o

sha.o: sha.h sha.c
	$(CC) $(CFLAGS) -c sha.c -o sha.o

sha256.o: sha256.h sha256.c
	$(CC) $(CFLAGS) -c sha256.c -o sha256.o

clean:
	rm -f $(EXEC) *.o *~

format:
	find . -name "*.c" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
	find . -name "*.h" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
