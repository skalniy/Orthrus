#include "acceptor.hpp"
#include <iostream>
#include <sstream>
#include <boost/bind.hpp>



Acceptor::Acceptor(std::shared_ptr<boost::asio::io_service> io_service) :
	acceptor(std::make_shared<boost::asio::ip::tcp::acceptor>(*io_service))	 
{
	std::cout << "Your name: ";
	std::getline(std::cin, name);

	std::cout << "Acceptor port: ";
	std::cin >> port;

	ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	acceptor->open(ep.protocol());
	acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor->bind(ep);
}


void Acceptor::start(void) 
{
	acceptor->listen();
	bool need_connect;
	std::string ip, p;
	std::cout << "connect to remote host?: ";
	std::cin >> need_connect;
	if (need_connect) {
		std::cout << "ip: ";
		std::cin >> ip;
		std::cout << "port: ";
		std::cin >> p;
		connect(ip, p);
	}

	std::thread w(&Acceptor::msg_proc, this);
	writer_thread.reset(new std::thread(move(w)));

	init();
}


void Acceptor::init(void)
{
	std::shared_ptr<boost::asio::ip::tcp::socket> sock =
		std::make_shared<boost::asio::ip::tcp::socket>(acceptor->get_io_service());
	acceptor->async_accept(
		*sock.get(), 
		boost::bind(&Acceptor::accept_handler, this, _1, sock)
	);
}


void Acceptor::accept_handler(const boost::system::error_code& error,
	std::shared_ptr<boost::asio::ip::tcp::socket> sock) 
{
	if (error) {
		std::cout << "accept error: " << error.message() << std::endl;
		return;
	}
	init();
	handshake(sock);
}


void Acceptor::handshake(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
	boost::asio::write(*sock, boost::asio::buffer(name+'\n'));
	boost::asio::write(*sock, boost::asio::buffer(std::to_string(port)+'\n'));
	std::shared_ptr<Peer> peer = std::make_shared<Peer>(sock);
	share_peers(peer->sock);
	peer->listen();
	g_peers.insert(peer);

	std::stringstream sst;
	sst << sock->remote_endpoint().address() << ":" << peer->port;
	std::string s;
	std::getline(sst, s);
	g_eps.insert(s);
}


void Acceptor::msg_proc(void) 
{
	while (true) {
		std::string msg;
		std::getline(std::cin, msg);
		for (auto peer : g_peers)
			boost::asio::async_write(*peer->sock, boost::asio::buffer(msg+'\n'), 
				boost::bind(&Peer::write_handler, peer, _1, _2));
	}
}


void Acceptor::connect(std::string peer_ip, std::string peer_port) 
{	
	boost::asio::ip::tcp::resolver resolver(acceptor->get_io_service());
	boost::asio::ip::tcp::resolver::query query(
		boost::asio::ip::tcp::v4(), peer_ip, peer_port);
	boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint ep = *it;
	
	std::shared_ptr<boost::asio::ip::tcp::socket> sock = 
		std::make_shared<boost::asio::ip::tcp::socket>(acceptor->get_io_service());
	sock->connect(ep);
	std::cerr<< "connected to " << ep << std::endl;

	std::shared_ptr<Peer> peer = std::make_shared<Peer>(sock);	
	boost::asio::write(*sock, boost::asio::buffer(name+'\n'));
	boost::asio::write(*sock, boost::asio::buffer(std::to_string(port) + '\n'));
	g_peers.insert(peer);

	std::stringstream sst;
	sst << sock->remote_endpoint().address() << ":" << peer->port;
	std::string s;
	std::getline(sst, s);
	g_eps.insert(s);

	read_peers(peer->sock);
	peer->listen();
}


void Acceptor::share_peers(std::shared_ptr<boost::asio::ip::tcp::socket> target) 
{
	std::stringstream s1;
	s1 << g_peers.size();
	std::string st1;
	s1 >> st1;
	boost::asio::write(*target, boost::asio::buffer('\n'+st1+'\n'));
	for (auto peer : g_peers) {
		std::stringstream st;
		st << peer->sock->remote_endpoint().address() << " " 
			<< peer->port;
		std::string st2;
		std::getline(st, st2);
		boost::asio::write(*target, boost::asio::buffer(st2+'\n'));
	}
}


void Acceptor::read_peers(std::shared_ptr<boost::asio::ip::tcp::socket> sock) 
{
	boost::asio::streambuf buf;
	std::string misc;
	boost::asio::read_until(*sock, buf, '\n');
	std::istream ist(&buf);
	std::getline(ist, misc);
	size_t peers_amount;
	ist >> peers_amount;
	for (auto i = 0; i < peers_amount; i++) {
		boost::asio::read_until(*sock, buf, '\n');
		std::getline(ist, misc);
		boost::asio::read_until(*sock, buf, '\n');
		std::string p_ip, p_port,addr;
		ist >> p_ip >> p_port;
		addr = p_ip+":"+p_port;
		if (g_eps.find(addr) == g_eps.end()) 
			connect(p_ip, p_port);
	}
}
