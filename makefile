FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: main.o utils.o spmat.o test.o
	gcc main.o utils.o spmat.o test.o - cluster $(LIBS)
clean:
	rm -rf *.o cluster

main.o: main.c utils.h
	gcc $(FLAGS) -c main.c
spmat.o: spmat.c spmat.h
	gcc $(FLAGS) -c spmat.c
utils.o: utils.c utils.h spmat.h
	gcc $(FLAGS) -c utils.c
test.o: test.c utils.h
	gcc $(FLAGS) -c test.c