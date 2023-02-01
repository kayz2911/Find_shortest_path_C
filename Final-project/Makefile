output: dllist.o file_handler.o graph.o jrb.o jval.o generate.o TSP.o Sort.o Main.o
	gcc dllist.o file_handler.o graph.o jrb.o jval.o generate.o TSP.o Sort.o Main.o -o output -w

Main.o: Main.c
	gcc -c Main.c -w

Sort.o: Sort.c
	gcc -c Sort.c -w
dllist.o: dllist.c dllist.h
	gcc -c dllist.c -w

file_handler.o: file_handler.c file_handler.h
	gcc -c file_handler.c -w

graph.o: graph.c graph.h
	gcc -c graph.c -w

jrb.o: jrb.c jrb.h
	gcc -c jrb.c -w

jval.o: jval.c jval.h
	gcc -c jval.c -w

generate.o: generate.c generate.h
	gcc -c generate.c -w

TSP.o: TSP.c TSP.h
	gcc -c TSP.c -w

clean:
	rm *.o