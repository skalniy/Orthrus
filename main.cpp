#include "router_controller.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <thread>



using namespace std;
using namespace Orthrus;


void msg_proc(RouterController::send_t write) 
{
    while (true) {
        std::string msg;
        std::getline(std::cin, msg);
        if (msg == "#exit")
        	return;
        write(msg);
    }
}


void error_handler(exception& e)
{ cerr << e.what() << endl; }


void accepted(std::string name)
{ cout << name << " joined the room" << endl; }


void read_msg(std::string nick, std::string msg)
{ cout << '[' << nick << "] " << msg << endl;}


int main(int argc, char const *argv[]) try
{
	if (!((argc == 3) || (argc == 1))) {
		cerr << "Usage: orthrus <host> <port>" << endl;
		return 1;
	}

	cout << "Hostname: ";
	string str, hname;
	unsigned short port;
	getline(cin, str);
	hname = str;
	cout << "Host port: ";
	cin >> port;
	getline(cin, str);

    RouterController s(hname, port, &error_handler);
    s.set_read_msg_cb(&read_msg);   
    s.start();
    if (argc == 3)
    	s.connect(argv[1], argv[2]);

    std::unique_ptr<std::thread> writer_thread;
    writer_thread.reset(new std::thread(&msg_proc, s.send));
    writer_thread->join();

    s.stop();

    return 0;
}
catch (std::exception& e) { 
    error_handler(e);
    return 1; 
}
