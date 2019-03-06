#include "net.h"

// STRUCT udp::SingleSyncEchoServer

udp::SingleSyncEchoServer::SingleSyncEchoServer(
    const unsigned short portNum) :
    server_{ portNum }
{}

udp::SingleSyncEchoServer::~SingleSyncEchoServer() = default;

std::string udp::SingleSyncEchoServer::response()
{
    std::string raw_ip;
    unsigned short port;
    std::string message{ server_.recv(raw_ip, port) };

    server_.send(raw_ip, port, message);

    return message;
}

// STRUCT udp::SingleSyncEchoClient

udp::SingleSyncEchoClient::SingleSyncEchoClient() = default;

udp::SingleSyncEchoClient::~SingleSyncEchoClient() = default;

void udp::SingleSyncEchoClient::setEndpoint(const std::string & rawIp,
    const unsigned short portNum)
{
    raw_ip_ = rawIp;
    port_ = portNum;
}

std::string udp::SingleSyncEchoClient::request(const std::string & message)
{
    client_.send(raw_ip_, port_, message);

    return client_.recv(raw_ip_, port_);
}

// STRUCT tcp::SingleSyncEchoServer

tcp::SingleSyncEchoServer::SingleSyncEchoServer(
    const unsigned short portNum) :
    server_{ portNum }
{}

tcp::SingleSyncEchoServer::~SingleSyncEchoServer() = default;

std::string tcp::SingleSyncEchoServer::response()
{
    std::string message{ server_.recv() };

    server_.send(message);

    return message;
}

// STRUCT tcp::SingleSyncEchoClient

tcp::SingleSyncEchoClient::SingleSyncEchoClient() = default;

tcp::SingleSyncEchoClient::~SingleSyncEchoClient() = default;

void tcp::SingleSyncEchoClient::setEndpoint(const std::string & rawIp,
    const unsigned short portNum)
{
    client_.connect(rawIp, portNum);
}

std::string tcp::SingleSyncEchoClient::request(const std::string & message)
{
    client_.send(message);

    return client_.recv();
}
