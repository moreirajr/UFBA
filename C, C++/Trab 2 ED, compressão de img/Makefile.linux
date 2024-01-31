.c.o:   $*.h
	gcc -c $*.c

.cpp.o:	$*.h
	g++	-c $*.cpp

all:	quadtree

quadtree:  quadtree.o glm.o winGL.o 
	gcc -o $@ $^ -lglut -lGL -lGLU

clean:
	rm *.o *.*~ *~ quadtree