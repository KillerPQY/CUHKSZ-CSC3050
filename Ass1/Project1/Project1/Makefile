tester:tester.o tools.o
	g++ tester.o tools.o -o tester
tester.o:tester.cpp
	g++ -c tester.cpp -o tester.o
tools.o:tools.cpp tools.h
	g++ -c tools.cpp -o tools.o

.PHONY:clean

clean:
	rm *.o tester