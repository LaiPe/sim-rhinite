OPT = -Wall -g

all: rhinite

rhinite : rhinite.o mt19937ar.o menuing.o csv.o affichage.o
	gcc $(OPT) rhinite.o mt19937ar.o menuing.o csv.o affichage.o -o rhinite -lm


rhinite.o: source/rhinite.c
	gcc $(OPT) -c source/rhinite.c

mt19937ar.o: packages/mt19937ar/mt19937ar.c packages/mt19937ar/mt19937ar.h
	gcc $(OPT) -c packages/mt19937ar/mt19937ar.c

menuing.o: packages/menuing/menuing.c packages/menuing/menuing.h
	gcc $(OPT) -c packages/menuing/menuing.c

csv.o: packages/csv/csv.c packages/csv/csv.h
	gcc $(OPT) -c packages/csv/csv.c

affichage.o: packages/affichage/affichage.c packages/affichage/affichage.h
	gcc $(OPT) -c packages/affichage/affichage.c



clean:
	rm -rf rhinite.o mt19937ar.o menuing.o affichage.o csv.o rhinite out/

dev: rhinite.o mt19937ar.o menuing.o csv.o affichage.o
	gcc $(OPT) rhinite.o mt19937ar.o menuing.o csv.o affichage.o -o rhinite -lm -p