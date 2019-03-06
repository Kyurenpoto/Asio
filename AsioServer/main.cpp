#include <iostream>

#include "../AsioWrapLib/net.h"

template<class TypeEchoServer>
int testEchoServer()
{
    try
    {
        TypeEchoServer server{ 3333 };

        std::cout << "Received message from client: " << server.response() << "\n";

        return 0;
    }
    catch (std::runtime_error & e)
    {
        std::cout << e.what() << "\n";

        return 1;
    }
}

int main()
{
    if (testEchoServer<udp::SingleSyncEchoServer>() ||
        testEchoServer<tcp::SingleSyncEchoServer>())
        return 1;

    return 0;
}
