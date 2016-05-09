all: check_heap


check_heap: heap.o
	gcc -std=c11 heap_main.c -g heap.o

heap.o: ../utils/heap.c
	gcc ../utils/heap.c -g -c


