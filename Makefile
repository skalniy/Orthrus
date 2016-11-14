all: peer.o router.o server.o main.cpp 
	g++ main.cpp peer.o router.o server.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system -lpthread	-std=c++1y


peer.o: peer.hpp peer.cpp
	g++ -c peer.cpp	-o peer.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


router.o: router.hpp router.cpp peer.hpp
	g++ -c router.cpp	-o router.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


server.o: server.hpp server.cpp router.hpp
	g++ -c server.cpp	-o server.o -I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


test: all
	./a.out


clean:
	rm *.so *.o *.out
