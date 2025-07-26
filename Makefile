calana: main.o
	gcc main.o -o calana

main.o: main.c
	gcc -Wall -Werror -pedantic -O3 -ggdb -c main.c

clean:
	rm main.o calana
