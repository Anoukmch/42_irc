#include <iostream>
#include <string>
#include <csignal>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

bool server_shutdown = false;

static void	signal_handler(int signal)
{
	(void)signal;
	server_shutdown = true;
}

bool ValidPort(std::string port, float &port_nbr)
{
    for(unsigned int i = 0; i < port.size(); i++)
    {
        if(std::isdigit(port[i]) == false)
            return(false);
    }
    std::istringstream iss(port);
    iss >> port_nbr;
    if(port_nbr <= 0 || port_nbr > MAX_PORT_NUMBER)
        return(false);
    return (true);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        std::cout << "Error: Wrong number of arguments" << std::endl;
    else
    {
        float port;
        if (ValidPort(argv[1], port) == false)
        {
            std::cout << "Port Error: Invalid Port" << std::endl;
            return(1);
        }
		signal(SIGINT, signal_handler);
        Server serv(htons((uint16_t)port), argv[2]);
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
