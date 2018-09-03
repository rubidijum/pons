PROGRAM = pons
CC = gcc
LDLIBS = -lglut -lGLU -lGL

$(PROGRAM): main.o help.o bridge.o
	$(CC) $^ -o $@ $(LDLIBS) -lm

main.o: main.c
	$(CC) -c $< $(LDLIBS) -lm

bridge.o: bridge.c bridge.h colors.h
	$(CC) -c $^ $(LDLIBS) -lm
	
help.o: help.c help.h colors.h
	$(CC) -c $^ $(LDLIBS) -lm

	
	
.PHONY: clean	

clean:
	rm *.o pons *.gch
