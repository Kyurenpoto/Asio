#pragma once

#include <string>

#include "../AsioWrapLib/netbase.h"

struct EchoServer
{
    virtual ~EchoServer() = default;

    virtual std::string response() = 0;
};

struct EchoClient
{
    virtual ~EchoClient() = default;

    virtual void setEndpoint(const std::string & rawIp,
        const unsigned short portNum) = 0;
    virtual std::string request(const std::string & message) = 0;
};

namespace udp
{
    struct SingleSyncEchoServer
        : public EchoServer
    {
        SingleSyncEchoServer(const unsigned short portNum);
        ~SingleSyncEchoServer();

        std::string response() override;

    private:
        single_sync_server server_;
    };

    struct SingleSyncEchoClient
        : public EchoClient
    {
        SingleSyncEchoClient();
        ~SingleSyncEchoClient();

        void setEndpoint(const std::string & rawIp,
                         const unsigned short portNum) override;
        std::string request(const std::string & message) override;

    private:
        single_sync_client client_;
        std::string raw_ip_;
        unsigned short port_;
    };
}

namespace tcp
{
    struct SingleSyncEchoServer
        : public EchoServer
    {
        SingleSyncEchoServer(const unsigned short portNum);
        ~SingleSyncEchoServer();

        std::string response() override;

    private:
        single_sync_server server_;
    };

    struct SingleSyncEchoClient
        : public EchoClient
    {
        SingleSyncEchoClient();
        ~SingleSyncEchoClient();

        void setEndpoint(const std::string & rawIp,
                         const unsigned short portNum) override;
        std::string request(const std::string & message) override;

    private:
        single_sync_client client_;
    };
}
