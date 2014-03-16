CC=gcc
FLAGS=-g
FILES=cprog.c 
OBJ=cprog.o 
OUT=c.out

c.out: build

build: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(OUT)

cprog.o: cprog.c header.h
	$(CC) $(FLAGS) -c  cprog.c

clean:
	rm -rf $(OBJ) $(OUT)

rebuild: clean build
