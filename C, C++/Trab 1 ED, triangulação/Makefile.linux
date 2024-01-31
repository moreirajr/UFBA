.c.o:   $*.h
	gcc -c $*.c

.cpp.o:	$*.h
	g++	-c $*.cpp

all:	trabalho

trabalho:  trabalho.o Triangulacao.o opGeometricas.o winGL.o 
	gcc -o $@ $^ -lglut -lGL -lGLU

clean:
	rm *.o *.*~ *~ trabalho