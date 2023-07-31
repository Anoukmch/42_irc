/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/07/31 16:59:01 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    std::cout << "Default Constructor" << std::endl;
}

Client::Client(int fd, std::string password) : ClientFd_(fd), pwd_(password)
{
    std::cout << "Default Constructor" << std::endl;
}

Client::Client(const Client &copyclass)
{
    std::cout << "Copy Constructor" << std::endl;
    *this = copyclass;
}

Client& Client::operator= (const Client& copyop)
{
    std::cout << "Copy Assignment Operator" << std::endl;
    if(this != &copyop)
    {
        nickname_ = copyop.nickname_;
        username_ = copyop.username_;
    }
    return(*this);
}

Client::~Client()
{
    std::cout << "Destructor" << std::endl;
}


// SETTER

void Client::set_nickname(std::string& nickname)
{
    nickname_ = nickname;
}
void Client::set_username(std::string& username)
{
    username_ = username;
}

// GETTER

std::string Client::get_nickname()
{
    return(nickname_);
}

std::string Client::get_username()
{
    return(username_);
}

// OTHER

void Client::ConnectionClosing()
{
    
}

void Client::ReceiveCommand()
{
    std::cout << "Receive Command" << std::endl;
    char buffer[512];
    ssize_t received = recv(ClientFd_, buffer, sizeof(buffer), 0);
    if (received <= 0)
        return ;
    buffer[received] = '\0';
    std::cout << "BUFFER: " << buffer << std::endl;
    CheckCommand(buffer);
    
}
void Client::CheckCommand(std::string buf)
{
    if(buf.substr(0, buf.find(' ')) == "PASS")
        PassCommand(buf.substr(buf.find(' ')+1, buf.find('\n')));
}

void Client::PassCommand(std::string buf)
{
    std::cout << "BUF: " << buf << std::endl;
    if (pwd_ != buf)
        std::cout << "WRONG PWD" << std::endl;
    // std::cout << buf.size() << std::endl;
        
}

void Client::SendData()
{
    
}
