CC=gcc
FLAGS=-g
FILES=Calculon.c SuiteAPI.c 
OBJ=Calculon.o SuiteAPI.o 
OUT=a.out

all: build

build: $(OBJ)
	$(CC) $(FLAGS) $(OBJ)

Calculon.o: Calculon.c SuiteAPI.h
	$(CC) $(FLAGS) -c  Calculon.c

SuiteAPI.o: SuiteAPI.c SuiteAPI.h
	$(CC) $(FLAGS) -c  SuiteAPI.c

clean:
	rm -rf $(OBJ) $(OUT)

rebuild: clean build
