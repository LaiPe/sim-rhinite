OPT = -Wall -g

all: rhinite

rhinite : rhinite.o mt19937ar.o menuing.o csv.o
	gcc $(OPT) rhinite.o mt19937ar.o menuing.o csv.o -o rhinite

rhinite.o: source/rhinite.c
	gcc $(OPT) -c source/rhinite.c

mt19937ar.o: packages/mt19937ar/mt19937ar.c packages/mt19937ar/mt19937ar.h
	gcc $(OPT) -c packages/mt19937ar/mt19937ar.c

menuing.o: packages/menuing/menuing.c packages/menuing/menuing.h
	gcc $(OPT) -c packages/menuing/menuing.c

csv.o: packages/csv/csv.c packages/csv/csv.h
	gcc $(OPT) -c packages/csv/csv.c

clean:
	rm -f rhinite.o mt19937ar.o menuing.o rhinite