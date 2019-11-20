all: myBackoff

myBackoff: myBackoff.o
	g++ myBackoff.cpp -o myBackoff
	
clean:
	\rm *.o myBackoff