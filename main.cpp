#include <iostream>
#include <string>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// ./ircserv <port> <password>
int main(int argc, char *argv[])
{
    if (argc != 3)
        std::cout << "Error: Wrong number of arguments" << std::endl;
    else
    {
        Server serv(htons(std::atoi(argv[1])), argv[2]);
        try
        {
            serv.server_setup();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
}