shell: main.o
	g++ main.o -o shell

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o shell
