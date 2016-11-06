all: peer.o acceptor.o server.o main.cpp 
	g++ main.cpp peer.o acceptor.o server.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-lpthread	-std=c++1y


peer.o: peer.hpp peer.cpp
	g++	-c peer.cpp	-o peer.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y

acceptor.o: acceptor.hpp acceptor.cpp peer.hpp
	g++	-c acceptor.cpp -o acceptor.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


server.o: server.hpp server.cpp acceptor.hpp
	g++	-c server.cpp	-o server.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


test: all
	./a.out


clean:
	rm *.so *.o *.out
	