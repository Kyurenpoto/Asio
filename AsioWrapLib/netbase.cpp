#include "netbase.h"

#include <string>
#include <array>
#include <optional>
#include <stdexcept>

#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include <boost/asio.hpp>

using namespace boost;

// STRUCT udp::single_sync_server::Impl

struct udp::single_sync_server::Impl
{
    Impl(const unsigned short portNum) :
        sock_{ ios_, asio::ip::udp::endpoint{asio::ip::udp::v4(), portNum} }
    {}

    ~Impl()
    {
        close();
    }

    void send(const std::string & rawIp,
              const unsigned short portNum,
              const std::string & message)
    {
        sendRequest(asio::ip::udp::endpoint{
            asio::ip::address::from_string(rawIp), portNum },
            message);
    }

    std::string recv(std::string & rawIp,
                     unsigned short & portNum)
    {
        asio::ip::udp::endpoint ep;
        std::string message{ receiveResponse(ep) };

        rawIp = ep.address().to_string();
        portNum = ep.port();

        return message;
    }

private:
    void close()
    {
        sock_.shutdown(asio::ip::udp::socket::shutdown_both);
        sock_.close();
        ios_.stop();
    }

    void sendRequest(const asio::ip::udp::endpoint & ep,
                     const std::string & message)
    {
        sock_.send_to(asio::buffer(message), ep);
    }

    std::string receiveResponse(asio::ip::udp::endpoint & ep)
    {
        size_t bytes{ sock_.receive_from(asio::buffer(buf_, 1024), ep) };

        return std::string{ buf_.data(), bytes };
    }

    asio::io_service ios_;
    asio::ip::udp::socket sock_;
    std::array<char, 1024> buf_;
};

// STRUCT udp::single_sync_server

udp::single_sync_server::single_sync_server(const unsigned short portNum) :
    pImpl{ std::make_unique<Impl>(portNum) }
{}

udp::single_sync_server::~single_sync_server() = default;

void udp::single_sync_server::send(const std::string & rawIp,
                                   const unsigned short portNum,
                                   const std::string & message)
{
    try
    {
        pImpl->send(rawIp, portNum, message);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

std::string udp::single_sync_server::recv(std::string & rawIp,
                                          unsigned short & portNum)
{
    try
    {
        return pImpl->recv(rawIp, portNum);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

// STRUCT udp::single_sync_client::Impl

struct udp::single_sync_client::Impl
{
    Impl() :
        sock_{ ios_ }
    {
        sock_.open(asio::ip::udp::v4());
    }

    ~Impl()
    {
        close();
    }

    void send(const std::string & rawIp,
              const unsigned short portNum,
              const std::string & message)
    {
        sendRequest(asio::ip::udp::endpoint{
            asio::ip::address::from_string(rawIp), portNum },
            message);
    }

    std::string recv(std::string & rawIp,
                     unsigned short & portNum)
    {
        asio::ip::udp::endpoint ep;
        std::string message{ receiveResponse(ep) };

        rawIp = ep.address().to_string();
        portNum = ep.port();

        return message;
    }

private:
    void close()
    {
        sock_.shutdown(asio::ip::udp::socket::shutdown_both);
        sock_.close();
        ios_.stop();
    }

    void sendRequest(const asio::ip::udp::endpoint & ep,
                     const std::string & message)
    {
        sock_.send_to(asio::buffer(message), ep);
    }

    std::string receiveResponse(asio::ip::udp::endpoint & ep)
    {
        size_t bytes{ sock_.receive_from(asio::buffer(buf_, 1024), ep) };

        return std::string{ buf_.data(), bytes };
    }

    asio::io_service ios_;
    asio::ip::udp::socket sock_;
    std::array<char, 1024> buf_;
};

// STRUCT udp::single_sync_client

udp::single_sync_client::single_sync_client() :
    pImpl{ std::make_unique<Impl>() }
{}

udp::single_sync_client::~single_sync_client() = default;

void udp::single_sync_client::send(const std::string & rawIp,
                                   const unsigned short portNum,
                                   const std::string & message)
{
    try
    {
        pImpl->send(rawIp, portNum, message);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

std::string udp::single_sync_client::recv(std::string & rawIp,
                                          unsigned short & portNum)
{
    try
    {
        return pImpl->recv(rawIp, portNum);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

// STRUCT tcp::single_sync_server::Impl

struct tcp::single_sync_server::Impl
{
    Impl(const unsigned short portNum) :
        acc_{ ios_, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), portNum} },
        sock_{ acc_.accept() }
    {}

    ~Impl()
    {
        close();
    }

    void send(const std::string & message)
    {
        sock_.send(asio::buffer(message));
    }

    std::string recv()
    {
        system::error_code ec;
        std::optional<std::string> message;

        while (true)
        {
            message = receiveResponse(ec);

            if (ec.value() == 0)
                break;

            if (ec.value() == asio::error::eof)
                sock_ = acc_.accept();
        }

        return message.value();
    }

private:
    void close()
    {
        sock_.shutdown(asio::ip::udp::socket::shutdown_both);
        sock_.close();
        acc_.close();
        ios_.stop();
    }

    std::optional<std::string> receiveResponse(system::error_code & ec)
    {
        size_t bytes{ sock_.receive(asio::buffer(buf_), 0, ec) };
        
        if (ec.value() != 0)
            return std::nullopt;

        return std::string{ buf_.data(), bytes };
    }

    asio::io_service ios_;
    asio::ip::tcp::acceptor acc_;
    asio::ip::tcp::socket sock_;
    std::array<char, 1024> buf_;
};

// STRUCT tcp::single_sync_server

tcp::single_sync_server::single_sync_server(const unsigned short portNum) :
    pImpl{ std::make_unique<Impl>(portNum) }
{}

tcp::single_sync_server::~single_sync_server() = default;

void tcp::single_sync_server::send(const std::string & message)
{
    try
    {
        pImpl->send(message);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

std::string tcp::single_sync_server::recv()
{
    try
    {
        return pImpl->recv();
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

// STRUCT tcp::single_sync_client::Impl

struct tcp::single_sync_client::Impl
{
    Impl() :
        sock_{ ios_ }
    {}

    ~Impl()
    {
        close();
    }

    void connect(const std::string & rawIp,
                 const unsigned short portNum)
    {
        sock_.connect(asio::ip::tcp::endpoint{
            asio::ip::address::from_string(rawIp), portNum });
    }

    void send(const std::string & message)
    {
        sock_.send(asio::buffer(message));
    }

    std::string recv()
    {
        size_t bytes{ sock_.receive(asio::buffer(buf_)) };

        return std::string{ buf_.data(), bytes };
    }

private:
    void close()
    {
        sock_.shutdown(asio::ip::udp::socket::shutdown_both);
        sock_.close();
        ios_.stop();
    }

    asio::io_service ios_;
    asio::ip::tcp::socket sock_;
    std::array<char, 1024> buf_;
};

// STRUCT tcp::single_sync_client

tcp::single_sync_client::single_sync_client() :
    pImpl{ std::make_unique<Impl>() }
{}

tcp::single_sync_client::~single_sync_client() = default;

void tcp::single_sync_client::connect(const std::string & rawIp,
                                      const unsigned short portNum)
{
    try
    {
        pImpl->connect(rawIp, portNum);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

void tcp::single_sync_client::send(const std::string & message)
{
    try
    {
        pImpl->send(message);
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}

std::string tcp::single_sync_client::recv()
{
    try
    {
        return pImpl->recv();
    }
    catch (system::system_error & e)
    {
        throw std::runtime_error{
            "Error occured(" + std::to_string(e.code().value()) +
            "): " + e.what() };
    }
}
