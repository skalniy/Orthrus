#include "router_controller.hpp"
#include <iostream>
#include <string>



using namespace std;


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
    while (true);
    s.stop();

    return 0;
}
