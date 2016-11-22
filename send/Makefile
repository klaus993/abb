CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
FILEOUT = pruebas.out
VALGRIND = valgrind
TDA = abb
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
VALGRIND_V = $(VALGRIND) $(VFLAGS)

all:
	$(CC) $(CFLAGS) -o $(FILEOUT) *.c

# all: main.o pruebas_alumno.o $(TDA).o $(EXTRA_O) testing.o
# 	$(CC) $(CFLAGS) -o $(FILEOUT) main.o pruebas_alumno.o $(TDA).o testing.o $(LFLAGS) 

# debug: main.o pruebas_alumno.o $(TDA).o $(EXTRA_O) testing.o
# 	$(CC) $(CFLAGS) $(DEBUG) -o $(FILEOUT) main.o pruebas_alumno.o $(TDA).o testing.o $(LFLAGS) 

# main.o: main.c $(TDA).h testing.h
# 	$(CC) $(CFLAGS) -c -o main.o main.c $(LFLAGS)

# pruebas_alumno.o: pruebas_alumno.c $(TDA).h $(EXTRA_H) testing.h
# 	$(CC) $(CFLAGS) -c -o pruebas_alumno.o pruebas_alumno.c $(LFLAGS)

# pila.o: pila.c pila.h
# 	$(CC) $(CFLAGS) -c -o pila.o pila.c $(LFLAGS)

# $(TDA).o: $(TDA).c $(TDA).h pila.h
# 	$(CC) $(CFLAGS) -c -o $(TDA).o $(TDA).c $(LFLAGS)

# testing.o: testing.c testing.h
# 	$(CC) $(CFLAGS) -c -o testing.o testing.c $(LFLAGS)

# abb: abb.o pila.o

clean:
	rm *.o
	rm -r *.dSYM
	rm *.out

test:
	$(VALGRIND_V) ./$(FILEOUT) 

run:
	./$(FILEOUT)
