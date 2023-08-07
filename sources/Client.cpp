/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/07 13:36:59 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    std::cout << "Default Constructor" << std::endl;
}

// Initialization of all attribute

Client::Client(int fd, Server* server) : ClientFd_(fd), ClientState_(-1), username_("Unknown"), isop_(false), server_(server)
{
    std::cout << "Constructor" << std::endl;
}

Client::Client(const Client &copyclass) : ClientFd_(copyclass.ClientFd_)
        , ClientState_(copyclass.ClientState_), server_(copyclass.server_)
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

void Client::set_output(std::string mess)
{
    output_ = output_.append(mess);
}

void Client::set_opflag(bool flag)
{
    isop_ = flag;
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

int Client::get_state()
{
    return(ClientState_);
}

int Client::get_fd()
{
    return(ClientFd_);
}

bool Client::get_opflag()
{
    return(isop_);
}

// OTHER

void Client::ConnectionClosing()
{

}

// END OF MESSAGE ALWAYS \r\n ????
// Check if command is wrong, example "PASSpwd" (no space between command and param) or Nick (no capital letter)
void Client::ReceiveCommand()
{
    // std::cout << "Receive Command" << std::endl;
    char buffer[512];
    ssize_t received = recv(ClientFd_, buffer, sizeof(buffer), 0);
    if (received <= 0)
        return ;
    buffer[received] = '\0';
    buffer_ = std::string(buffer);

    // FOR WEECHAT
    size_t rc = buffer_.find("\r\n");
    if(rc != std::string::npos)
        buffer_ = buffer_.substr(0, rc);

    // FOR NC
    size_t nl = buffer_.find("\n");
    if(nl != std::string::npos)
        buffer_ = buffer_.substr(0, nl);

    CheckCommand(buffer_);
}

// CHECK SCREENSHOT TERMINAL : The server is receiving somethign after sending a message to client ?

void Client::SendData()
{
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

    // GET COMMAND
    if (buf.find(' ') == std::string::npos)
    {
        cmd_ = buf;
        std::cout << "Command: " << cmd_ << std::endl;
        for(unsigned int i = 0; i < params_.size(); i++)
            std::cout << "Param[" << i << "]: " << params_[i] << std::endl;
        std::cout << "Trailing: " << trailing_ << std::endl;
        return ;
    }
    else
        cmd_ = buf.substr(0, buf.find(' '));

    // GET PARAM STRING & TRAILING
    if(buf.find(':') == std::string::npos)
        tmp = buf.substr(buf.find(' ') + 1, buf.size());
    else
    {
        tmp = buf.substr(buf.find(' ') + 1, buf.find(':') - (buf.find(' ')+1));
        trailing_ = buf.substr(buf.find(':'), buf.size()-(buf.find(':')));
    }

    // GET SPLITTED PARAMS
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
    output_ = Messages::ERR_UNKNOWNCOMMAND(nickname_, cmd_);
}

// COMMANDS

void Client::PassCmd()
{
    if ( ClientState_ >= REGISTERED)
        output_ = Messages::ERR_ALREADYREGISTRED();
    else if (ClientState_ == PASS)
        output_ = "Client already authenticated"; // Can we create an error message?
    else if (params_.empty())
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if (server_->CheckPassword(params_[0]) == false)
        output_ = Messages::ERR_PASSWDMISMATCH();
    else if (params_.size() != 1)
        output_ = Messages::ERR_PASSWDMISMATCH();
    else
        ClientState_ = PASS;
}

void Client::CapCmd()
{
    // output_ = Messages::RPL_CAP();
}

void Client::NickCmd()
{
    if (ClientState_ < PASS)
        output_ = Messages::ERR_NOTREGISTERED(cmd_);
    else if(params_.empty() == true)
        output_ = Messages::ERR_NONICKNAMEGIVEN();
    // ONLY 8 CHARACTERS ????
    // CHECK FOR SOME SPECIAL SIGNS,?? ...

    else if(server_->IsUniqueNickname(params_[0]) == false)
        output_ = Messages::ERR_NICKNAMEINUSE(params_[0]);
    else
    {
        if (nickname_.empty() == false)
            output_ = Messages::RPL_NICKCHANGE(nickname_, params_[0], username_);
        else if (ClientState_ == PASS && username_ != "Unknown")
        {
            ClientState_ = REGISTERED;
            // output_ = Messages::RPL_WELCOME(nickname_, username_);
        }
        nickname_ = params_[0];
    }
}

// Does the trailing need to start with ":" ?
// How to change your username afterwards ? Can we use USER after registration ?
// Error message "used invalid mode flags" pb when nickname is not set

void Client::UserCmd()
{
    if (params_.size() != 3 || trailing_.empty())
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if ((params_[1] != "0" && params_[1] != "*")
        || (params_[2] != "0" && params_[2] != "*"))
        output_ = Messages::ERR_UMODEUNKNOWNFLAG(nickname_);
    else if (ClientState_ < PASS)
        output_ = Messages::ERR_NOTREGISTERED(cmd_);
    // else if ( ClientState_ >= REGISTERED)
    //     output_ = Messages::ERR_ALREADYREGISTRED(); // Do I need the Append function here?
    else
    {
        if (!nickname_.empty())
        {
            ClientState_ = REGISTERED;
            output_ = Messages::RPL_WELCOME(nickname_, username_);
        }
        username_ = params_[0];
    }
}

// CHANNEL = FULL?, TOO MANY CHANNELS?, INVITEONLY CHANNEL?
// Channel already exists, channel has key that user doesn't know
//  /JOIN 0 ->leave all channels
// IF CHANNEL DOESN'T EXIST ->CLIENT BECOMES OPERATOR
void Client::JoinCmd()
{
    if(params_.size() < 1 || params_.size() > 2)
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    std::vector<std::string>::iterator it;
    std::vector<std::string>keys;
    if(params_.size() == 2)
    {
        std::stringstream key(params_[1]);
        std::string ke;
        while(getline(key, ke, ','))
            keys.push_back(ke);
        it = keys.begin();
    }
    std::stringstream name(params_[0]);
    std::string token;
    while(getline(name, token, ','))
    {
        Channel* exist = server_->GetChannel(token);
        if(token[0] != '&' && token[0] != '#')
            output_ = output_.append(Messages::ERR_NOSUCHCHANNEL(nickname_, token)); // Do we need the Append function here?
        else if(exist != nullptr)
        {
            // CHECK FOR KEY FI ITS THE SAME, ..
            // if(exist->get_key != "" )
            // {

            // }
            exist->AddClientToChannel(this);
            exist->SendMessageToChannel(Messages::RPL_JOIN_OR(nickname_, username_, token), this);
            output_ = output_.append(Messages::RPL_JOIN(nickname_, username_, token));
        }
        else
        {
            server_->AddChannel(token);
            server_->GetLastChannel()->AddClientToChannel(this);
            server_->GetLastChannel()->set_inviteonlyflag(false);
            server_->GetLastChannel()->set_operator(this->get_nickname());
            // SET CHANNELOPERATOR
            channels_.push_back((server_->GetLastChannel()));

            if (keys.empty()== false && it != keys.end())
            {
                server_->GetLastChannel()->set_key(*it);
                output_ = output_.append(Messages::RPL_JOIN_WITHKEY(nickname_, username_, token, *it)); //MULTIPLE MESSAGES!!!!
                it++;
            }
            else
                output_ = output_.append(Messages::RPL_JOIN(nickname_, username_, token)); //MULTIPLE MESSAGES!!!!
        }
    }
}

// Where does the RPL and ERR from Messages.cpp come from? Which RFC protocol?

void Client::PingCmd()
{
    //   Parameters: <server1> [ <server2> ]

    //   Numeric Replies:
    //   ERR_NOORIGIN : Is it necessary to specify a prefix?
    /*
        The prefix is used by servers to indicate the true
        origin of the message.
        If the prefix is missing from the message, it
        is assumed to have originated from the connection from which it was
        received from. ---> So the flag ERR_NOORIGIN is useless bc the origin specification is optional?
    */
    //   ERR_NOSUCHSERVER,
    //   RPL_PING (Where is it coming from?)

    // Is it possible to PING without parameter? Is it necessary to specify the servername?

    if (params_.empty())
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if (params_[0] != SERVERNAME)
         output_ = Messages::ERR_NOSUCHSERVER(nickname_, params_[0]);
    else
        output_ = Messages::RPL_PING(nickname_);
    // The parameter doesnt match the server name : err_nosuchserver
    //
}

void Client::ModeCmd()
{

}

void Client::NamesCmd()
{

}

// PART MESSAGE
// ERR_NOSUCHCHANNEL
// ERR_NOTONCHANNEL
// leave other channels if one is wrong???
// /PART channels(with ,) [part message]
// IS PART MESSAGE NECESSARY???
void Client::PartCmd()
{
    if(params_.size() != 1)
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        std::stringstream name(params_[0]);
        std::string token;
        while(getline(name, token, ','))
        {
            Channel* c = server_->GetChannel(token);
            if(c == nullptr)
                output_ = output_.append(Messages::ERR_NOSUCHCHANNEL(nickname_, token));
            else if(c->IsClientOnChannel(this) == false)
                output_ = output_.append(Messages::ERR_NOTONCHANNEL(nickname_, token));
            else
            {
                c->SendMessageToChannel(Messages::RPL_PART_OR(nickname_, username_, token, trailing_), this);
                output_ = output_.append(Messages::RPL_PART(nickname_, username_, token, trailing_));
                c->RemoveClientFromChannel(this);
                if(c->IsChannelNotEmpty() == false)
                    server_->DeleteChannel(token);
            }
        }
    }
}

void Client::PrivmsgCmd()
{
    if(params_.size() != 1 || trailing_ == "")
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        // MESSAGE TO CHANNEL
        if(params_[0][0] == '#' || params_[0][0] == '&')
        {
            Channel *chan = server_->GetChannel(params_[0]);
            if(chan == nullptr)
                output_ = Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
            else
                chan->SendMessageToChannel(Messages::RPL_PRIVMSG(nickname_, username_, chan->get_name(), &trailing_[1]), this);
        }
        else
        {
            // MESSAGE TO CLIENT
            Client *cli = server_->GetClient(params_[0]);
            if(cli == nullptr)
                output_ = Messages::ERR_NOSUCHNICK_NICKONLY(nickname_);
            else
                cli->set_output(Messages::RPL_PRIVMSG(nickname_, username_, cli->get_nickname(), &trailing_[1]));
        }
    }

}

// if invite-only flag -> only channel operators may invite others
// only invited user and the inviting user will receive the message
void Client::InviteCmd()
{
    if(params_.size() != 2 || trailing_ != "")
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        Client *c = server_->GetClient(params_[0]);
        Channel *chan = server_->GetChannel(params_[1]);
        if(c == nullptr)
            output_ = Messages::ERR_NOSUCHNICK_NICKONLY(params_[0]);
        else if(chan == nullptr || chan->IsClientOnChannel(this) == false)
            output_ = Messages::ERR_NOTONCHANNEL(nickname_, params_[1]);
        else if (chan->IsClientOnChannel(c) == true)
            output_ = Messages::ERR_USERONCHANNEL(nickname_, params_[0], params_[1]);
        else
        {
            c->set_output(Messages::RPL_INVITED(nickname_, username_, params_[1], params_[0]));
            output_ = Messages::RPL_INVITING(nickname_, params_[1], params_[0]);
        }
    }
}

void Client::TopicCmd()
{
    if(params_.size() < 1 || params_.size() > 1
        || (params_.size() != 1 && trailing_ != ""))
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    Channel* c = server_->GetChannel(params_[0]);
    if (c == nullptr)
    {
        output_ = Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
        return ;
    }
    if(params_.size() == 1)
    {
        if(trailing_ == "")
        {
            if(c->get_topic() == "")
                output_ = Messages::RPL_NOTOPIC(nickname_, params_[0]);
            else
                output_ = Messages::RPL_TOPIC(nickname_, params_[0], c->get_topic());
        }
        else if(trailing_ == ":")
        {
            std::string clear = "";
            c->set_topic(clear);
            output_ = Messages::RPL_TOPICCHANGE(nickname_, username_, params_[0], clear);
        }
        else if(trailing_.size() > 1)
        {
            c->set_topic(&trailing_[1]);
            output_ = Messages::RPL_TOPICCHANGE(nickname_, username_, params_[0], &trailing_[1]);
        }
    }
}

// <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]
void Client::KickCmd()
{
    if(params_.size() != 2)
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        // ONLY CHANNEL OPERATOR IS ALLOWED TO KICK ANOTHER USER OUT OF A CHANNEL
        // ERR_USERNOTINCHANNEL
        // ERR_NOSUCHCHANNEL
        // ERR_NOTONCHANNEL (USER THAT WANTS TO KICK SOMEONEELSE)
    }
}

void Client::OperCmd()
{
    if(params_.size() != 2 || trailing_ != "")
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        Client* c = server_->GetClient(params_[0]);
        if(c == nullptr)
            output_ = Messages::ERR_NOSUCHNICK_NICKONLY(nickname_);
        else if (params_[1] != OPERPWD)
            output_ = Messages::ERR_PASSWDMISMATCH();
        else
        {
            output_ = Messages::RPL_YOUREOPER(nickname_, params_[0]);
            c->set_opflag(true);
        }
    }
}

void Client::NoticeCmd()
{

}

void Client::QuitCmd()
{
    if(params_.size() > 0)
    {
        output_ = Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }

    if(channels_.empty() == false)
    {
        std::vector<Channel*>::iterator it = channels_.begin();
        while(it != channels_.end())
        {
            (*it)->RemoveClientFromChannel(this);
            it++;
        }
    }
    if(params_.size() == 0)
    {
        if (trailing_ == "")
            output_ = Messages::RPL_QUIT(nickname_, username_);
        else
            output_ = Messages::RPL_QUIT_MESSAGE(nickname_, username_, trailing_);
        ClientState_ = DISCONNECTED;
    }
}
