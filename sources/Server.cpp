/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/07/31 16:56:40 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server()
{
    serverSocket_ = 0;
    std::cout << "Default Constructor" << std::endl;
}

Server::Server(const Server &copyclass)
{
    std::cout << "Copy Constructor" << std::endl;
    *this = copyclass;
}

Server& Server::operator= (const Server& copyop)
{
    std::cout << "Copy Assignment Operator" << std::endl;
    if(this != &copyop)
    {
        // topic_ = copyop.topic_;
        // mode_ = copyop.mode_;
    }
    return(*this);
}

Server::~Server()
{
    std::cout << "Destructor" << std::endl;
    close(serverSocket_); //this is a c function, use another one from c++!!!
}

Server::Server(uint16_t port, std::string password) : port_(port), connection_pd_(password)
{
    serverSocket_ = 0;
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = port_;
    // INPUT HANDLING
}

void Server::server_setup()
{
    if ((serverSocket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Opening Socket ";
        throw SetupError();
    }
    int opted = 1;
    if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)))
    {
        std::cout << "Socket ";
        throw SetupError();
    }
    if (bind(serverSocket_, (struct sockaddr*)&address_, sizeof(address_)) < 0)
    {
        std::cout << "Binding Socket ";
        throw SetupError();
    }
    if (listen(serverSocket_, SOMAXCONN) == -1)
    {
        std::cout << "Listen from Server ";
        throw SetupError();
    }

    pollfd tmp = {.revents = 0, .events = EVENTS, .fd = serverSocket_};
    PollStructs_.push_back(tmp);
    
}

void Server::MainLoop()
{
    while(true)
    {
        if(poll(PollStructs_.data(), PollStructs_.size(), 100) == -1)
            return ;
        if(PollStructs_[0].revents & POLLIN)
            acceptConnection();
        for(unsigned int i = 1; i < PollStructs_.size(); i++)
        {
            if(PollStructs_[i].revents & POLLERR)
                ConnectedClients_[i-1]->ConnectionClosing();
            else if(PollStructs_[i].revents & POLLIN)
                ConnectedClients_[i-1]->ReceiveCommand();
            if(PollStructs_[i].revents & POLLOUT)
                ConnectedClients_[i-1]->SendData();
        }
        CheckForDisconnections();
    }
}

void Server::acceptConnection()
{
    // ACCEPT MUST BE IN WHILE LOOP while(true)
    // When does while end ????
    // accept function blocks until a client connects to the server
    int address_length = sizeof(address_);
    int new_client_fd = accept(serverSocket_, (struct sockaddr*)&address_, (socklen_t*)&address_length);
    if (new_client_fd == -1)
    {
        std::cout << "Accept ";
        throw SetupError();
    }
    pollfd tmp = {.revents = 0, .events = EVENTS, .fd = new_client_fd};
    PollStructs_.push_back(tmp);
    ConnectedClients_.push_back(new Client(new_client_fd, connection_pd_));
    std::cout << "Accept Connection" << std::endl;
}

void Server::CheckForDisconnections()
{
    
}

// // SETTER

// void Server::set_topic(std::string& topic)
// {
//     topic_ = topic;
// }
// void Server::set_mode(std::string& mode)
// {
//     mode_ = mode;
// }

// // GETTER

std::string Server::getPassword()
{
    return(connection_pd_);
}

// std::string Server::get_topic()
// {
//     return(topic_);
// }

// std::string Server::get_mode()
// {
//     return(mode_);
// }

// EXCEPTIONS

const char* Server::SetupError::what() const throw()
{
	return("Error: Check Server Settings, set up not possible!");
}