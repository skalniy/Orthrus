#include "router.hpp"
#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>



Router::Router(std::shared_ptr<boost::asio::io_service> io_service) :
    acceptor(std::make_shared<boost::asio::ip::tcp::acceptor>(*io_service))  
{
    std::cout << "Hostname: ";
    std::getline(std::cin, hostname);

    std::cout << "Acceptor port: ";
    unsigned short port;
    std::cin >> port;

    ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor->open(ep.protocol());
    acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor->bind(ep);
}


void Router::start() 
{
    acceptor->listen();
    std::string addr;
    std::cout << "connect to remote host?" << std::endl << "IPv4:port / 0: ";
    std::cin >> addr;
    if (addr != "0")
        connect(addr);

    writer_thread.reset(new std::thread(&Router::msg_proc, this));

    init();
}


void Router::stop()
{
    if (!acceptor->is_open())
        return;
    acceptor->cancel();
    acceptor->close();
}


void Router::init()
{
    std::shared_ptr<boost::asio::ip::tcp::socket> sock =
        std::make_shared<boost::asio::ip::tcp::socket>(
            acceptor->get_io_service()
        );

    acceptor->async_accept(
        *sock, 
        boost::bind(&Router::accept_handler, this, _1, sock)
    );
}


void Router::accept_handler(const boost::system::error_code& error,
    std::shared_ptr<boost::asio::ip::tcp::socket> sock) 
{
    if (error) {
        std::cout << "accept error: " << error.message() << std::endl;
        return;
    }
    init();

    boost::asio::write(*sock, boost::asio::buffer(hostname + '\n'));
    boost::asio::write(*sock, boost::asio::buffer(std::to_string(ep.port()) + '\n'));
    std::shared_ptr<Peer> new_peer = std::make_shared<Peer>(sock);
    new_peer->listen();
    share_peers(new_peer->get_sock());
    peers.emplace(new_peer->get_remote_address(), new_peer);
}


void Router::msg_proc() 
{
    while (true) {
        std::string msg;
        std::getline(std::cin, msg);
        for (auto peer_ : peers)
            peer_.second->write(msg+'\n'); 
    }
}


void Router::connect(std::string addr) 
{
    std::vector<std::string> ep_params;
    ep_params = 
        boost::algorithm::split(ep_params, addr, boost::algorithm::is_any_of(":"));
    boost::asio::ip::tcp::endpoint remote_ep = 
        *boost::asio::ip::tcp::resolver(acceptor->get_io_service()).resolve(
                boost::asio::ip::tcp::resolver::query(
                    boost::asio::ip::tcp::v4(), ep_params[0], ep_params[1]
                )
            );
    
    std::shared_ptr<boost::asio::ip::tcp::socket> sock = 
        std::make_shared<boost::asio::ip::tcp::socket>(acceptor->get_io_service());
    sock->connect(remote_ep);
    std::cout << "connected to " << remote_ep << std::endl;

    std::shared_ptr<Peer> peer = std::make_shared<Peer>(sock);  
    boost::asio::write(*peer->get_sock(), boost::asio::buffer(hostname + '\n'));
    boost::asio::write(*peer->get_sock(), boost::asio::buffer(std::to_string(ep.port()) + '\n'));

    peers.emplace(peer->get_remote_address(), peer);
    read_peers(peer->get_sock());
    peer->listen();
}


void Router::share_peers(std::shared_ptr<boost::asio::ip::tcp::socket> target) 
{
    boost::asio::write(*target, boost::asio::buffer(std::to_string(peers.size()) + '\n'));
    for (auto peer : peers)
        boost::asio::write(*target, 
            boost::asio::buffer(peer.second->get_remote_address() + '\n')
        );
}


void Router::read_peers(std::shared_ptr<boost::asio::ip::tcp::socket> sock) 
{
    boost::asio::streambuf buf;
    std::istream ist(&buf);

    boost::asio::read_until(*sock, buf, '\n');
    size_t peers_amount;
    ist >> peers_amount;
    for (auto i = 0; i < peers_amount; i++) {
        std::string misc, addr;

        boost::asio::read_until(*sock, buf, '\n');
        std::getline(ist, misc);
        boost::asio::read_until(*sock, buf, '\n');
        std::getline(ist, addr);

        if (peers.find(addr) == peers.end()) 
            connect(addr);
    }
}
