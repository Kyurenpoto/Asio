#include <iostream>

#include "../AsioWrapLib/net.h"

template<class TypeEchoClient>
int testEchoClient()
{
    try
    {
        TypeEchoClient client;

        client.setEndpoint("127.0.0.1", 3333);
        std::cout << "Sending message to server: test\n";
        std::cout << "Received message from server: " <<
            client.request("test") << "\n";

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
    if (testEchoClient<udp::SingleSyncEchoClient>() ||
        testEchoClient<tcp::SingleSyncEchoClient>())
        return 1;

    return 0;
}
