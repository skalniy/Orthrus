#include "router_controller.hpp"
#include <iostream>
#include <functional>
#include <string>



using namespace std;


void msg_proc(RouterController::write_t write) 
{
    while (true) {
        std::string msg;
        std::getline(std::cin, msg);
        if (msg == "exit")
        	return;
        write(msg);
    }
}


int main(int argc, char const *argv[])
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

    RouterController s(hname, port);
    s.start();
    if (argc == 3)
    	s.connect(argv[1], argv[2]);

    std::unique_ptr<std::thread> writer_thread;
    writer_thread.reset(new std::thread(&msg_proc, s.write));
    writer_thread->join();
    s.stop();

    return 0;
}
