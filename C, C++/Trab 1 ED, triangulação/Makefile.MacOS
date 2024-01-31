.c.o:   $*.h
	gcc -c $*.c

.cpp.o:	$*.h
	g++	-c $*.cpp

all:	trabalho

trabalho:  trabalho.o Triangulacao.o opGeometricas.o winGL.o 
	gcc -o $@ $^ -framework OpenGL -framework GLUT

clean:
	rm *.o *.*~ *~ trabalho