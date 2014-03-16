CC=gcc
FLAGS=-g
FILES=dprog.c 
OBJ=dprog.o 
OUT=a.out

a.out: main.c

fdksjl: $(OBJ)
	$(CC) $(FLAGS) $(OBJ)

dprog.o: dprog.c header.h
	$(CC) $(FLAGS) -c  dprog.c

clean:
	rm -rf $(OBJ) $(OUT)

rebuild: clean build
