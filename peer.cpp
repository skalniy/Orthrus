#include "peer.hpp"
#include <unordered_set>



std::unordered_set<std::shared_ptr<Peer>> g_peers;


Peer::Peer(std::shared_ptr<boost::asio::ip::tcp::socket> sock_) : sock(sock_)
{
	ist = std::make_shared<std::iostream>(buf.get());
	boost::asio::read_until(*sock, *buf, '\n');
	std::getline(*ist, name);
	boost::asio::read_until(*sock, *buf, '\n');
	std::getline(*ist, port);
	std::cerr << "joined " << name <<":" << port << std::endl;
}


Peer::~Peer()
{
	sock->close();
	std::cout << "socket closed" << std::endl;
}


void Peer::listen(void) 
{
	std::cerr << "start listen" << std::endl;
	boost::asio::async_read_until(*sock, *buf, '\n',
		boost::bind(&Peer::read_handler, this, _1, _2));
}


void Peer::read_handler(const boost::system::error_code& ec,
	std::size_t bytes_transferred)
{
	if (ec) {
		std::cout << "error: " << ec.message() << std::endl;
		return;
	}

	std::string msg;
	std::getline(*ist, msg);
	boost::asio::async_read_until(*sock, *buf, '\n', 
		boost::bind(&Peer::read_handler, this, _1, _2));
	printf("[%s] %s\n", name.c_str(), msg.c_str());
}


void Peer::write_handler(const boost::system::error_code& error, 
  		std::size_t bytes_transferred)
{
	if (error) {
		std::cout << "write error: " << error.message() << std::endl;
		return;
	}
}
