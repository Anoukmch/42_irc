/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/07/27 15:15:17 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    server_fd_ = 0;
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
    close(server_fd_); //this is a c function, use another one from c++!!!
}

Server::Server(uint16_t port, std::string password) : port_(port), connection_pd_(password)
{
    server_fd_ = 0;
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = port_;
    // INPUT HANDLING
}

void Server::server_setup()
{
    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Opening Socket ";
        throw SetupError();
    }
    int opted = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)))
    {
        std::cout << "Socket ";
        throw SetupError();
    }
    if (bind(server_fd_, (struct sockaddr*)&address_, sizeof(address_)) < 0)
    {
        std::cout << "Binding Socket ";
        throw SetupError();
    }
    if (listen(server_fd_, SOMAXCONN) == -1)
    {
        std::cout << "Listen from Server ";
        throw SetupError();
    }
    acceptConnection();
}

void Server::acceptConnection()
{
    int address_length = sizeof(address_);
    if ((server_sock_ = accept(server_fd_, (struct sockaddr*)&address_, (socklen_t*)&address_length)) < 0)
    {
        std::cout << "Accept ";
        throw SetupError();
    }
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