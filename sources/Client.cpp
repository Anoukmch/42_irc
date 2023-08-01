/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/01 14:45:34 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    std::cout << "Default Constructor" << std::endl;
}

Client::Client(int fd, std::string password) : ClientFd_(fd), pwd_(password)
{
    std::cout << "Constructor" << std::endl;
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
        // nickname_ = copyop.nickname_;
        // username_ = copyop.username_;
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

void Client::SendData()
{
    if(output_.empty())
        return ;
    send(ClientFd_, output_.data(), output_.size(), 0);
}

void Client::CheckCommand(std::string buf)
{
    std::string cmd = buf.substr(0, buf.find(' '));
    params_ = buf.substr(buf.find(' ')+1, buf.size());

    std::string cmds[16] = { "PASS", "CAP", "NICK", "USER", "JOIN", "PING", "MODE",
        "NAMES", "PART", "PRIVMSG", "INVITE", "TOPIC", "KICK", "OPER", "NOTICE", "QUIT"};
	void (Client::*fp[16])(void) = {&Client::PassCmd, &Client::CapCmd, &Client::NickCmd,
        &Client::UserCmd, &Client::JoinCmd, &Client::PingCmd, &Client::ModeCmd, &Client::NamesCmd,
        &Client::PartCmd, &Client::PrivmsgCmd, &Client::InviteCmd, &Client::TopicCmd,
        &Client::KickCmd, &Client::OperCmd, &Client::NoticeCmd, &Client::QuitCmd};

    for(int i = 0; i < 16; i++)
    {
        if(cmd == cmds[i])
        {
            (this->*fp[i])();
            return ;
        }
    }
}

// COMMANDS

void Client::PassCmd()
{
    // if user not connected check that PASS cmd is there
    
    if (params_.empty())
        std::cout << "NO PARAMS";
    if (pwd_ != params_)
    {
        
        std::cout << "WRONG PWD" << std::endl;
    }
    // Check for multiple params, ...
}

void Client::CapCmd()
{
    
}

void Client::NickCmd()
{
    if(params_.empty() == true)
    {
        output_ = Messages::ERR_NONICKNAMEGIVEN();
        return ;
    }

    // ONLY 8 CHARACTERS ????
    // CHECK FOR SOME SPECIAL SIGNS, ...
    // if(params_.find(' ') != std::string::npos
    //     || params_.find(',') != std::string::npos || params_.find('?') != std::string::npos
    //     || params_.find('!') != std::string::npos || params_.find('@') != std::string::npos
    //     || params_.find('.') != std::string::npos || params_[0] == '$' || params_[0] == ':'
    //     || params_[0] == '&' || params_[0] == '#')
    // {
    //     output_ = Messages::ERR_ERRONEUSNICKNAME(nickname_, params_);
    //     return ;
    // }

    // CHECK FOR UNIQUE NICKNAME
    // ERR_NICKCOLLISION
    
    // if(Server::IsUniqueNickname(params_) == false)
    //     std::cout << "WORKS!" << std::endl;
    std::cout << "HERE" << std::endl;
    if (nickname_.empty() == true)
    {
        nickname_ = params_;
        output_ = Messages::RPL_WELCOME(nickname_, username_);
    }
    else if (nickname_.empty() == false)
    {
        output_ = Messages::RPL_NICKCHANGE(nickname_, params_, username_);
        nickname_ = params_;
    }
}

void Client::UserCmd()
{
    
}

void Client::JoinCmd()
{
    
}

void Client::PingCmd()
{
    
}

void Client::ModeCmd()
{
    
}

void Client::NamesCmd()
{
    
}

void Client::PartCmd()
{
    
}

void Client::PrivmsgCmd()
{
    
}

void Client::InviteCmd()
{
    
}

void Client::TopicCmd()
{
    
}

void Client::KickCmd()
{
    
}

void Client::OperCmd()
{
    
}

void Client::NoticeCmd()
{
    
}

void Client::QuitCmd()
{
    
}
