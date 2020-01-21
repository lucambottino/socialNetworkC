all:
	gcc $(wildcard *.c *.h) -o socialmanager.run -std=c99 -Wall -Werror -g
run:
	./socialmanager.run
