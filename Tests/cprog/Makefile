CC=gcc
FLAGS=-g
FILES=cprog.c 
OBJ=cprog.o 
OUT=a.out

all: build

build: $(OBJ)
	$(CC) $(FLAGS) $(OBJ)

cprog.o: cprog.c header.h
	$(CC) $(FLAGS) -c  cprog.c

clean:
	rm -rf $(OBJ) $(OUT)

rebuild: clean build
