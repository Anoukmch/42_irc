/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amechain <amechain@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/03 09:17:14 by amechain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    std::cout << "Default Constructor" << std::endl;
}

Client::Client(int fd) : ClientFd_(fd), ClientState_(-1)
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
    // std::cout << "Receive Command" << std::endl;
    char buffer[512];
    ssize_t received = recv(ClientFd_, buffer, sizeof(buffer), 0);
    if (received <= 0)
        return ;
    buffer[received] = '\0';
    buffer_ = std::string(buffer);

    size_t rc = buffer_.find("\r\n");
    if(rc != std::string::npos)
        buffer_ = buffer_.substr(0, rc);

    // std::cout << "buffer_: " << buffer_ << std::endl;

    CheckCommand(buffer_);

}

void Client::SendData()
{
    while(params_.empty() != true)
        params_.pop_back();
    params_.clear();
    cmd_ = "";
    trailing_ = "";
    buffer_ = "";

    if(output_.empty())
        return ;

    send(ClientFd_, output_.data(), output_.size(), 0);
    output_ = "";
}

void Client::SetCmdParamsTrailing(std::string buf)
{
    std::string tmp;
    if (buf.find(' ') == std::string::npos)
    {
        cmd_ = buf;
        return ;
    }
    else
        cmd_ = buf.substr(0, buf.find(' '));

    if(buf.find(':') == std::string::npos)
        tmp = buf.substr(buf.find(' ') + 1, buf.size());
    else
    {
        tmp = buf.substr(buf.find(' ') + 1, buf.find(':') - (buf.find(' ')+1));
        trailing_ = buf.substr(buf.find(':'), buf.size());
    }

    std::istringstream stream(tmp);
    std::string token;
    while(stream >> token)
        params_.push_back(token);

    // PRINTING EVERYTHING CMD, PARAMS & TRAILING
    std::cout << "Command: " << cmd_ << std::endl;
    for(unsigned int i = 0; i < params_.size(); i++)
        std::cout << "Param[" << i << "]: " << params_[i] << std::endl;
    std::cout << "Trailing: " << trailing_ << std::endl;
}

void Client::CheckCommand(std::string buf)
{
    SetCmdParamsTrailing(buf);

    std::string cmds[16] = { "PASS", "CAP", "NICK", "USER", "JOIN", "PING", "MODE",
        "NAMES", "PART", "PRIVMSG", "INVITE", "TOPIC", "KICK", "OPER", "NOTICE", "QUIT"};
	void (Client::*fp[16])(void) = {&Client::PassCmd, &Client::CapCmd, &Client::NickCmd,
        &Client::UserCmd, &Client::JoinCmd, &Client::PingCmd, &Client::ModeCmd, &Client::NamesCmd,
        &Client::PartCmd, &Client::PrivmsgCmd, &Client::InviteCmd, &Client::TopicCmd,
        &Client::KickCmd, &Client::OperCmd, &Client::NoticeCmd, &Client::QuitCmd};

    for(int i = 0; i < 16; i++)
    {
        if(cmd_ == cmds[i])
        {
            (this->*fp[i])();
            return ;
        }
    }
}

// COMMANDS

void Client::PassCmd()
{
    // CHECK IF USER ALREADY REGISTERED
    // ERR_ALREADYREGISTERED

    if (params_.empty())
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    if (Server::CheckPassword(params_[0]) == false)
    {
        std::cout << "WRONG PWD" << std::endl;
    }
    // Check for multiple params, ...
    ClientState_ = PASS;
}

void Client::CapCmd()
{

}

void Client::NickCmd()
{
    if(params_.empty() == true)
        output_ = Messages::ERR_NONICKNAMEGIVEN();
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

    // PROBLEM WHEN CONNECTING WITH SAME NICK, SERVER CLOSES!!!!
    // CHECK FOR UNIQUE NICKNAME
    else if (ClientState_ < PASS)
        output_ = Messages::ERR_NOTREGISTERED(cmd_);
    else if(Server::IsUniqueNickname(params_[0]) == false)
        output_ = Messages::ERR_NICKNAMEINUSE(params_[0]);
    else if (nickname_.empty() == false)
        output_ = Messages::RPL_NICKCHANGE(nickname_, params_[0], username_);
    else if (ClientState_ == PASS && !username_.empty())
    {
        ClientState_ = REGISTERED;
        output_ = Messages::RPL_WELCOME(nickname_, username_);
    }
    nickname_ = params_[0];
}

void Client::UserCmd() // How to change your username afterwards ? Bc USER is just at the begining
{
    if (params_.size() != 3 || trailing_.empty())
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if ((params_[1] != "0" && params_[1] != "*")
        || (params_[2] != "0" && params_[2] != "*"))
        output_ = Messages::ERR_UMODEUNKNOWNFLAG(cmd_);
    else if (ClientState_ < PASS) // Is PASS = 0 ?
        output_ = Messages::ERR_NOTREGISTERED(cmd_);
    else if (!nickname_.empty())
    {
        ClientState_ = REGISTERED;
        output_ = Messages::RPL_WELCOME(nickname_, username_);
    }
    else if ( ClientState_ >= REGISTERED)
        output_ = Messages::ERR_ALREADYREGISTRED();
    username_ = trailing_;
}

// DIFFERENCE BETWEEN TOPIC AND NAME???
void Client::JoinCmd()
{
    if(params_.size() < 1)
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    if(params_[0][0] != '&' && params_[0][0] != '#')
    {
        // ADD ANOTHER ERROR MESSAGE HERE?
        // INVALID CHANNEL NAME ???
        output_ = Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
        return ;
    }
    Server::AddChannel(params_[0]);
    // CREATES CHANNEL
    output_ = Messages::RPL_JOIN(nickname_, username_, params_[0]);
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
    if(params_.size() < 1 || params_.size() > 2)
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    Channel* c = Server::GetChannel(params_[0]);
    if (c == nullptr)
    {
        output_ = Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
        return ;
    }
    if(params_.size() == 1)
    {
        if(c->get_topic() == "")
            output_ = Messages::RPL_NOTOPIC(nickname_, params_[0]);
        else
            output_ = Messages::RPL_TOPIC(nickname_, params_[0],c->get_topic());
        return ;
    }
    else if(params_.size() == 2 && trailing_ == "")
    {
        c->set_topic(params_[1]);
        output_ = Messages::RPL_TOPICCHANGE(nickname_, username_, params_[0], params_[1]);
    }
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
