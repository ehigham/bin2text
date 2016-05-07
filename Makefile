all: main

main: build/main.o
	gcc build/main.o -o build/main

build/main.o: src/main.c
	gcc -c src/main.c -I. -o build/main.o

clear:
	rm ./build/*
