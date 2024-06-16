OPT = -Wall -g

all: stats_sim idle


stats_sim : source/stats_sim.c rhinite.o csv.o mt19937ar.o
	gcc $(OPT) source/stats_sim.c rhinite.o csv.o mt19937ar.o -o stats_sim -lm

idle : source/idle.c rhinite.o menuing.o mt19937ar.o affichage.o
	gcc $(OPT) source/idle.c rhinite.o menuing.o mt19937ar.o affichage.o -o idle



rhinite.o: source/rhinite.c source/rhinite.h 
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
	rm -rf *.o rhinite stats_sim out/