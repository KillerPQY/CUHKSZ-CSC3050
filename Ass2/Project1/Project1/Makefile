simulator:simulator.o datasolver.o
	g++ simulator.o datasolver.o -o simulator
simulator.o:simulator.cpp simulator.h datasolver.h
	g++ -c simulator.cpp -o simulator.o
datasolver.o:datasolver.cpp datasolver.h
	g++ -c datasolver.cpp -o datasolver.o


.PHONY:clean

clean:
	rm *.o simulator
