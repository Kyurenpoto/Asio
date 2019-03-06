#pragma once

#include <string>
#include <memory>

namespace udp
{
    struct single_sync_server
    {
        single_sync_server(const unsigned short portNum);
        ~single_sync_server();

        void send(const std::string & rawIp,
                  const unsigned short portNum,
                  const std::string & message);
        std::string recv(std::string & rawIp,
                         unsigned short & portNum);

    private:
        struct Impl;

        std::unique_ptr<Impl> pImpl;
    };

    struct single_sync_client
    {
        single_sync_client();
        ~single_sync_client();

        void send(const std::string & rawIp,
                  const unsigned short portNum,
                  const std::string & message);
        std::string recv(std::string & rawIp,
                         unsigned short & portNum);

    private:
        struct Impl;

        std::unique_ptr<Impl> pImpl;
    };
}

namespace tcp
{
    struct single_sync_server
    {
        single_sync_server(const unsigned short portNum);
        ~single_sync_server();

        void send(const std::string & message);
        std::string recv();

    private:
        struct Impl;

        std::unique_ptr<Impl> pImpl;
    };

    struct single_sync_client
    {
        single_sync_client();
        ~single_sync_client();

        void connect(const std::string & rawIp,
                     const unsigned short portNum);
        void send(const std::string & message);
        std::string recv();

    private:
        struct Impl;

        std::unique_ptr<Impl> pImpl;
    };
}
