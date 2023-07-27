/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/07/27 17:10:24 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
    acceptConnection();
}

void Server::acceptConnection()
{
    // ACCEPT MUST BE IN WHILE LOOP while(true)
    // When does while end ????
    // accept function blocks until a client connects to the server
    int address_length = sizeof(address_);
    clientSocket_ = accept(serverSocket_, (struct sockaddr*)&address_, (socklen_t*)&address_length);
    if (clientSocket_ == -1)
    {
        std::cout << "Accept ";
        throw SetupError();
    }
    // EXAMPLE: SENDING MESSAGE TO CLIENT
    const char* message = "Welcome to the IRC server!\n";
    send(clientSocket_, message, strlen(message), 0);
    // EXAMPLE: RECEIVING DATA FROM CLIENT
    char buffer[1024];
    int bytesRead = recv(clientSocket_, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received from client: " << buffer << std::endl;
    } else if (bytesRead == 0) {
        // The client closed the connection
        std::cout << "Client disconnected." << std::endl;
    } else {
        // Error occurred while receiving
        std::cerr << "Error while receiving data from the client." << std::endl;
    }
    close (clientSocket_);
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