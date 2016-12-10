all: peer router router_controller main.cpp
	g++	main.cpp peer.o router.o router_controller.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system -lpthread	-std=c++1y


peer: peer.hpp peer.cpp
	g++	-c peer.cpp	-o peer.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


router: router.hpp router.cpp peer.hpp
	g++	-c router.cpp	-o router.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


router_controller: router_controller.hpp router_controller.cpp router.hpp
	g++	-c router_controller.cpp	-o router_controller.o	-I/usr/local/include	-L/usr/local/lib	-lboost_system	-std=c++1y


test: all
	./a.out


clean:
	rm *.so *.o *.out
