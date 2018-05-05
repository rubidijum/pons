pons: main.o help.o
	gcc main.o help.o -o pons -lglut -lGLU -lGL -lm

main.o: main.c
	gcc -c main.c -lglut -lGLU -lGL -lm
	
help.o: help.c help.h
	gcc -c help.c -lglut -lGLU -lGL -lm
	
clean:
	rm *.o pons
