#include "server.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_set>



int main(int argc, char const *argv[])
{
	Server s;
	s.start();
	std::this_thread::sleep_for(std::chrono::seconds(300));
	s.stop();
	return 0;
}
