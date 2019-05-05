PROGRAM = pons
CC = gcc
LDLIBS = -lglut -lGLU -lGL

$(PROGRAM): main.o help.o
	$(CC) $^ -o $@ $(LDLIBS) -lm

main.o: main.c
	$(CC) -c $< $(LDLIBS) -lm
	
help.o: help.c help.h
	$(CC) -c $^ $(LDLIBS) -lm

	
.PHONY: clean	

clean:
	rm *.o pons
