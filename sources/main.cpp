#include <iostream>
#include <string>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

bool ValidPort(std::string port)
{
    for(unsigned int i = 0; i < port.size(); i++)
    {
        if(std::isdigit(port[i]) == false)
            return(false);
    }
    if(std::atoi(port.c_str()) <= 0)
        return(false);
    return (true);
}

// ./ircserv <port> <password>
int main(int argc, char *argv[])
{
    if (argc != 3)
        std::cout << "Error: Wrong number of arguments" << std::endl;
    else
    {
        if (ValidPort(argv[1]) == false)
        {
            std::cout << "Port Error: Invalid Port" << std::endl;
            return(1);
        }
        Server serv(htons(std::atoi(argv[1])), argv[2]);
        try
        {
            serv.server_setup();
            serv.MainLoop();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return (0);
}