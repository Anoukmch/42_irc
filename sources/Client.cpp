/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/01 11:35:05 by jmatheis         ###   ########.fr       */
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

// END OF MESSAGE ALWAYS \r\n ????
void Client::ReceiveCommand()
{
    std::cout << "Receive Command" << std::endl;
    char buffer[512];
    ssize_t received = recv(ClientFd_, buffer, sizeof(buffer), 0);
    if (received <= 0)
        return ;
    buffer[received] = '\0';
    buffer_ = std::string(buffer);

    size_t rc = buffer_.find("\r\n");
    if(rc != std::string::npos)
        buffer_ = buffer_.substr(0, rc);

    std::cout << "buffer_: " << buffer_ << std::endl;

    CheckCommand(buffer_);
    
}
void Client::CheckCommand(std::string buf)
{
    std::string cmd = buf.substr(0, buf.find(' '));
    std::string params = buf.substr(buf.find(' ')+1, buf.size());
    if(cmd == "PASS")
        PassCommand(params);
}

//        ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
void Client::PassCommand(std::string buf)
{
    std::cout << "BUF SIZE: " << buf.size() << std::endl;
    if (pwd_ != buf)
        std::cout << "WRONG PWD" << std::endl; //SPECIAL MESSAGE

    std::cout << GREEN << "User: " << user << " succesfully registered to the server, using nick " << nick << "!" << RESET << "\n";
    output_ = std::string(":") + SERVERNAME + " 001 " + nick + " :Welcome to the ft_irc network " + nick + "!" + user + "@" + HOST + "\r\n";
    // std::cout << buf.size() << std::endl;
}

// std::string RPL_WELCOME(const std::string& nick, const std::string user) 
// {
// 	std::cout << GREEN << "User: " << user << " succesfully registered to the server, using nick " << nick << "!" << RESET << "\n";
// 	return std::string(":") + SERVERNAME + " 001 " + nick + " :Welcome to the ft_irc network " + nick + "!" + user + "@" + HOST + "\r\n";
// }

void Client::SendData()
{
    // if (output_.Empty())
	// 		return;
		// while (output_buffer.HoldsMessage())
		// {
		// 	std::string output = output_buffer.GetMessageCR();
		// 	send(GetFd(), output.data(), output.size(), 0);
		// }
    
    send(ClientFd_, output_.data(), output_.size(), 0);
}
