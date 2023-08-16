/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amechain <amechain@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/16 17:32:22 by amechain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

/***************************
 * ORTHODOX CANONICAL FORM *
 ***************************/

Client::Client()
{
    std::cout << "Default Constructor" << std::endl;
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
    { ;
    }
    return(*this);
}

Client::~Client()
{
    // std::cout << "Destructor" << std::endl;
}

/***************************
 * CONSTRUCTOR WITH PARAMS *
 ***************************/

// Initialization of all attribute
Client::Client(int fd, Server* server) : ClientFd_(fd), ClientState_(-1), server_(server), mode_('0')
{
    // std::cout << "Constructor" << std::endl;
}

/***************************
 *          SETTER         *
 ***************************/

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
    output_ += mess;
}

void Client::set_mode(unsigned char c)
{
    mode_ = c;
}

/***************************
 *          GETTER         *
 ***************************/

void Client::AddChatClient(Client* cl)
{
    chatclients_.push_back(cl);
}


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

unsigned char Client::get_mode()
{
    return(mode_);
}

/***************************
 *         CHECKER         *
 ***************************/

bool Client::HaveAlreadyChatted(Client* cl)
{
    if(chatclients_.size() == 0)
        return (false);
    for(unsigned int i = 0; i < chatclients_.size(); i++)
    {
        if(chatclients_[i] == cl)
            return(true);
    }
    return(false);
}

/***************************
 *     RECEIVE & SEND      *
 ***************************/

void Client::ReceiveCommand()
{
	std::string temp;
    char buffer[512];
	memset(buffer, 0, 512);
	ssize_t received = recv(ClientFd_, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (received <= 0)
	{
		this->ConnectionClosing();
		return ;
	}
	temp = std::string(buffer);
	size_t rc = temp.find("\n");
	if(rc != std::string::npos)
	{
		if (temp.find("\r") != std::string::npos)
			temp = temp.substr(0, rc - 1);
		else
			temp = temp.substr(0, rc);
		buffer_ += temp;
		CheckCommand(buffer_);
		buffer_ = "";
	}
	else
	{
		buffer_ += temp;
		return ;
	}
}


void Client::SendData()
{
    params_.clear();
    cmd_ = "";
    trailing_ = "";

    if(output_.empty())
        return ;
    send(ClientFd_, output_.data(), output_.size(), 0);
    buffer_.clear();
    output_ = "";
}

/***************************
 *     COMMAND PARSING     *
 ***************************/

void Client::SetCmdParamsTrailing(std::string buf)
{
    std::string tmp;

    // GET COMMAND
    if (buf.find(' ') == std::string::npos)
    {
        cmd_ = buf;
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
    std::cout << "Trailing: " << trailing_ << std::endl << std::endl;
}

void Client::CheckCommand(std::string buf)
{
    SetCmdParamsTrailing(buf);

    std::string cmds[15] = { "PASS", "CAP", "NICK", "USER", "JOIN", "PING", "MODE",
        "NAMES", "PART", "PRIVMSG", "INVITE", "TOPIC", "KICK", "QUIT", "NOTICE"};
	void (Client::*fp[15])(void) = {&Client::PassCmd, &Client::CapCmd, &Client::NickCmd,
        &Client::UserCmd, &Client::JoinCmd, &Client::PingCmd, &Client::ModeCmd, &Client::NamesCmd,
        &Client::PartCmd, &Client::PrivmsgCmd, &Client::InviteCmd, &Client::TopicCmd,
        &Client::KickCmd, &Client::QuitCmd, &Client::NoticeCmd};

    for(int i = 0; i < 15; i++)
    {
        if(cmd_ == cmds[i])
        {
            if (i >= 4 && ClientState_ < REGISTERED)
                output_ += Messages::ERR_NOTREGISTERED(cmd_);
            else if (i > 0 && ClientState_ < PASS)
                output_ += Messages::ERR_NOTREGISTERED(cmd_);
            else if (ClientState_ != DISCONNECTED)
            (this->*fp[i])();
            return ;
        }
    }
    output_ += Messages::ERR_UNKNOWNCOMMAND(nickname_, cmd_);
}


/***************************
 *         COMMANDS        *
 ***************************/

void Client::PassCmd()
{
    if (ClientState_ >= PASS)
        output_ += Messages::ERR_ALREADYREGISTRED();
    else if (params_.empty())
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if (server_->CheckPassword(params_[0]) == false)
        output_ += Messages::ERR_PASSWDMISMATCH();
    else if (params_.size() != 1 || !trailing_.empty())
        output_ += Messages::ERR_PASSWDMISMATCH();
    else
        ClientState_ = PASS;
}

void Client::CapCmd()
{
	if (params_[0] != "END")
    	output_ += Messages::RPL_CAP();
}

void Client::NickCmd()
{
    if(params_.empty() == true)
        output_ += Messages::ERR_NONICKNAMEGIVEN();
    else if (params_.size() != 1 || !trailing_.empty())
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if(server_->IsUniqueNickname(params_[0]) == false && params_[0] != nickname_)
        output_ += Messages::ERR_NICKNAMEINUSE(params_[0]);
    else
    {
        if (!nickname_.empty())
            output_ += Messages::RPL_NICKCHANGE(nickname_, params_[0], username_);
        else if (ClientState_ == PASS && !username_.empty())
        {
            ClientState_ = REGISTERED;
            output_ += Messages::RPL_WELCOME(params_[0], username_);
        }
        nickname_ = params_[0];
    }
}

void Client::UserCmd()
{
    if (params_.size() != 3 || trailing_.empty())
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if ((params_[1] != "0" && params_[1] != "*")
        || (params_[2] != "0" && params_[2] != "*"))
        output_ += Messages::ERR_UMODEUNKNOWNFLAG(nickname_);
    else if ( ClientState_ >= REGISTERED)
       output_ += Messages::ERR_ALREADYREGISTRED();
    else
    {
        username_ = params_[0];
        if (!nickname_.empty())
        {
            ClientState_ = REGISTERED;
            output_ += Messages::RPL_WELCOME(nickname_, username_);
        }
    }
}

void Client::JoinCmd()
{
    if(params_.size() < 1 || params_.size() > 2)
    {
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
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
    if(keys.size() == 0)
        it = keys.end();
    std::stringstream name(params_[0]);
    std::string token;
    while(getline(name, token, ','))
    {
        Channel* exist = server_->GetChannel(token);
        if(token[0] != '&' && token[0] != '#')
            output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, token);
        else if(exist != 0)
        {
			if (exist->IsClientOnChannel(this) == true)
				exist->SendMessageToChannel(Messages::ERR_USERONCHANNEL(nickname_, params_[0], params_[1]), 0);
			else if (exist->get_inviteonlyflag() == true && exist->IsClientInvited(nickname_) == false)
				output_ += Messages::ERR_INVITEONLYCHAN(nickname_, exist->get_name());
			else if (exist->get_userlimit() == exist->GetClientNumbers())
				output_ += Messages::ERR_CHANNELISFULL(nickname_, exist->get_name());
            else if(it == keys.end())
            {
                if(exist->get_key() != "")
                    output_ += Messages::ERR_BADCHANNELKEY(nickname_, exist->get_name());
                else
                {
                    exist->AddClientToChannel(this);
                    channels_.push_back(exist);
                    exist->SendMessageToChannel(Messages::RPL_JOIN_OR(nickname_, username_, token), this);
                    output_ += Messages::RPL_JOIN(nickname_, username_, token);
                    exist->SendMessageToChannel(Messages::RPL_NAMREPLY(nickname_, token, exist->GetChannelList()), 0);
					exist->SendMessageToChannel(Messages::RPL_ENDOFNAMES(nickname_, token), 0);
                }
            }
            else if (it != keys.end())
            {
                if(exist->get_key() != *it)
                    output_ += Messages::ERR_BADCHANNELKEY(nickname_, exist->get_name());
                else
                {
                    exist->AddClientToChannel(this);
                    channels_.push_back(exist);
                    exist->SendMessageToChannel(Messages::RPL_JOIN_OR(nickname_, username_, token), this);
                    output_ += Messages::RPL_JOIN_WITHKEY(nickname_, username_, token, *it);
                    exist->SendMessageToChannel(Messages::RPL_NAMREPLY(nickname_, token, exist->GetChannelList()), 0);
					exist->SendMessageToChannel(Messages::RPL_ENDOFNAMES(nickname_, token), 0);
                }
                it++;
            }
        }
        else
        {
            server_->AddChannel(token);
            server_->GetLastChannel()->AddClientToChannel(this);
            server_->GetLastChannel()->set_inviteonlyflag(false);
            server_->GetLastChannel()->AddClientAsOperator(this->get_nickname());
            channels_.push_back((server_->GetLastChannel()));
            if (keys.empty()== false && it != keys.end())
            {
                server_->GetLastChannel()->set_key(*it);
                output_ += Messages::RPL_JOIN_WITHKEY(nickname_, username_, token, *it);
                it++;
            }
            else
                output_ += Messages::RPL_JOIN(nickname_, username_, token);
			exist = server_->GetChannel(token);
			exist->SendMessageToChannel(Messages::RPL_NAMREPLY(nickname_, token, exist->GetChannelList()), 0);
			exist->SendMessageToChannel(Messages::RPL_ENDOFNAMES(nickname_, token), 0);
        }
    }
}

void Client::PingCmd()
{
    if (params_.empty())
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if (params_[0] != "localhost" && params_[0] != "127.0.0.1" && params_[0] != SERVERNAME)
         output_ += Messages::ERR_NOSUCHSERVER(nickname_, params_[0]);
    else
        output_ += Messages::RPL_PING(nickname_, params_[1]);
}

void Client::ModeCmd()
{
	Channel *c = server_->GetChannel(params_[0]);
    if (params_.empty())
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else if ((params_[0][0] != '#' && params_[0][0] != '&') || c == 0)
       output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
	else if (params_.size() == 1)
		output_ += Messages::RPL_CHANNELMODEIS(nickname_, params_[0], c->get_mode());
	else if (c->IsClientAnOperator(nickname_) == false)
		output_ += Messages::ERR_CHANOPRIVSNEEDED(nickname_, params_[0]);
	else if (params_[1].size() < 2)
		output_ += Messages::ERR_UMODEUNKNOWNFLAG(nickname_);
	else if (params_[1][0] != '+' && params_[1][0] != '-')
		output_ += Messages::ERR_UMODEUNKNOWNFLAG(nickname_);
	else
	{
		std::string letters = "itkol";
		std::string letters2 = "kol";
    	for(size_t i = 1; i < params_[1].size(); i++)
    	{
    	    if (letters.find(params_[1][i]) != std::string::npos)
    	    {
				int num = c->GetOperatorNumbers();
				std::string mod = "**";
				mod[0] = params_[1][0];
				mod[1] = params_[1][i];
				if (letters2.find(params_[1][i]) != std::string::npos && params_.size() < 3)
				{
					output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
					return ;
				}
    	        c->set_mode(mod, params_);
				if (num != c->GetOperatorNumbers())
				{
					c = server_->GetChannel(params_[0]);
					c->SendMessageToChannel(Messages::RPL_NAMREPLY(nickname_, c->get_name(), c->GetChannelList()), NULL);
					c->SendMessageToChannel(Messages::RPL_ENDOFNAMES(nickname_, c->get_name()), NULL);
				}
				output_ += Messages::RPL_SETMODECHANNEL(this->get_nickname(), params_[0], mod);
    	    }
			else
				output_ += Messages::ERR_UMODEUNKNOWNFLAG(nickname_);
    	}
		output_ += Messages::RPL_CHANNELMODEIS(nickname_, params_[0], c->get_mode());
	}
}

void Client::NamesCmd()
{
	Channel* ch = server_->GetChannel(params_[0]);
	ch->SendMessageToChannel(Messages::RPL_NAMREPLY(nickname_, params_[0], ch->GetChannelList()), 0);
	ch->SendMessageToChannel(Messages::RPL_ENDOFNAMES(nickname_, params_[0]), 0);
}

void Client::PartCmd()
{
    if(params_.size() != 1)
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        std::stringstream name(params_[0]);
        std::string token;
        while(getline(name, token, ','))
        {
            Channel* c = server_->GetChannel(token);
            if(c == 0)
                output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, token);
            else if(c->IsClientOnChannel(this) == false)
                output_ += Messages::ERR_NOTONCHANNEL(nickname_, token);
            else
            {
                c->SendMessageToChannel(Messages::RPL_PART_OR(nickname_, username_, token, trailing_), this);
                output_ += Messages::RPL_PART(nickname_, username_, token, trailing_);
                c->RemoveClientFromChannel(this);
                RemoveChannel(c);
            }
        }
    }
}

void	Client::NoticeCmd()
{
	if (params_.size() != 1 || trailing_ == "")
		return ;
	else
    {
        // NOTICE TO CHANNEL
        if(params_[0][0] == '#' || params_[0][0] == '&')
        {
            Channel *chan = server_->GetChannel(params_[0]);
            if(chan)
                chan->SendMessageToChannel(Messages::RPL_NOTICE(nickname_, username_, chan->get_name(), &trailing_[1]), this);
        }
        else
        {
            // NOTICE TO CLIENT
            Client *cli = server_->GetClient(params_[0]);
            if(cli)
                cli->set_output(Messages::RPL_NOTICE(nickname_, username_, cli->get_nickname(), &trailing_[1]));
        }
    }
}

void Client::PrivmsgCmd()
{
    if(params_.size() != 1 || trailing_ == "")
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        // MESSAGE TO CHANNEL
        if(params_[0][0] == '#' || params_[0][0] == '&')
        {
            Channel *chan = server_->GetChannel(params_[0]);
            if(chan == 0)
                output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
            else if (!chan->IsClientOnChannel(this))
                output_ += Messages::ERR_NOTONCHANNEL(nickname_, chan->get_name());
            else
                chan->SendMessageToChannel(Messages::RPL_PRIVMSG(nickname_, username_, chan->get_name(), &trailing_[1]), this);
        }
        else
        {
            // MESSAGE TO CLIENT
            Client *cli = server_->GetClient(params_[0]);
            if(cli == 0)
                output_ += Messages::ERR_NOSUCHNICK_NICKONLY(nickname_);
            else
			{
                cli->set_output(Messages::RPL_PRIVMSG(nickname_, username_, cli->get_nickname(), &trailing_[1]));
                if(HaveAlreadyChatted(cli) == false)
                {
				    output_ += Messages::RPL_PRIVMSG(nickname_, username_, cli->get_nickname(), &trailing_[1]);
                    AddChatClient(cli);
                    cli->AddChatClient(this);
                }
			}
        }
    }

}

void Client::InviteCmd()
{
    if(params_.size() != 2 || trailing_ != "")
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        Client *c = server_->GetClient(params_[0]);
        Channel *chan = server_->GetChannel(params_[1]);
        if(c == 0)
            output_ += Messages::ERR_NOSUCHNICK_NICKONLY(params_[0]);
        else if(chan == 0 || chan->IsClientOnChannel(this) == false)
            output_ += Messages::ERR_NOTONCHANNEL(nickname_, params_[1]);
        else if (chan->IsClientOnChannel(c) == true)
            output_ += Messages::ERR_USERONCHANNEL(nickname_, params_[0], params_[1]);
        else if(chan->get_inviteonlyflag() == true && chan->IsClientAnOperator(nickname_) == false)
            output_ += Messages::ERR_CHANOPRIVSNEEDED(nickname_, chan->get_name());
        else
        {
            c->set_output(Messages::RPL_INVITED(nickname_, username_, params_[1], params_[0]));
            output_ += Messages::RPL_INVITING(nickname_, params_[1], params_[0]);
			chan->AddClientAsInvited(params_[0]);
        }
    }
}

void Client::TopicCmd()
{
    if(params_.size() < 1 || params_.size() > 1
        || (params_.size() != 1 && trailing_ != ""))
    {
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
        return ;
    }
    Channel* c = server_->GetChannel(params_[0]);
    if (c == 0)
    {
        output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, params_[0]);
        return ;
    }
    if(params_.size() == 1)
    {
        if(trailing_ == "")
        {
            if(c->get_topic() == "")
                output_ += Messages::RPL_NOTOPIC(nickname_, params_[0]);
            else
                output_ += Messages::RPL_TOPIC(nickname_, params_[0], c->get_topic());
        }
        else if(trailing_ == ":")
        {
			if (c->IsClientAnOperator(nickname_) == false && c->get_topicflag() == true)
				output_ += Messages::ERR_CHANOPRIVSNEEDED(nickname_, params_[0]);
			else
			{
				std::string clear = "";
				c->set_topic(clear);
				c->SendMessageToChannel(Messages::RPL_TOPICCHANGE(nickname_, username_, params_[0], clear), 0);
			}
        }
        else if(trailing_.size() > 1)
        {
			if (c->IsClientAnOperator(nickname_) == false && c->get_topicflag() == true)
				output_ += Messages::ERR_CHANOPRIVSNEEDED(nickname_, params_[0]);
			else
			{
				c->set_topic(&trailing_[1]);
				c->SendMessageToChannel(Messages::RPL_TOPICCHANGE(nickname_, username_, params_[0], &trailing_[1]), 0);
			}
        }
    }
}

void Client::KickCmd()
{
    if(params_.size() != 2)
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
    else
    {
        if (params_[0].find(',') != std::string::npos && SameNumbChannelsClientsToKick() == false)
            output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
        else
        {
            std::stringstream channels(params_[0]);
            std::stringstream users(params_[1]);
            std::string channel;
            std::string user;
            while((getline(channels, channel, ',') && getline(users, user, ',')) || getline(users, user, ','))
            {
                Channel* channelptr = server_->GetChannel(channel);
                Client* client = server_->GetClient(user);
                if (channelptr == 0)
                    output_ += Messages::ERR_NOSUCHCHANNEL(nickname_, channel);
                else if (client == 0)
                    output_ += Messages::ERR_NOSUCHNICK_NICKONLY(user);
                else if(IsPossibleToKick(channelptr, client) == true)
                {
                    channelptr->RemoveClientFromChannel(client);
                    channelptr->RemoveClientAsOperator(client->get_nickname()); //possible?
                    client->RemoveChannel(channelptr);
                    if(trailing_ == "")
                        output_ += Messages::RPL_KICK(nickname_, username_, channel, user);
                    else
                        output_ += Messages::RPL_KICK_MESSAGE(nickname_, username_, channel, user, trailing_);
                }
            }
        }
    }
}

void Client::QuitCmd()
{
    if(params_.size() > 0)
    {
        output_ += Messages::ERR_NEEDMOREPARAMS(cmd_);
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
            output_ += Messages::RPL_QUIT(nickname_, username_);
        else
            output_ += Messages::RPL_QUIT_MESSAGE(nickname_, username_, trailing_);
        ConnectionClosing();
    }
}

/***************************
 *    COMMAND HELPERS      *
 ***************************/

bool Client::IsPossibleToKick(Channel* channelptr, Client* client)
{
    if (channelptr->IsClientOnChannel(this) == false)
    {
        output_ += Messages::ERR_NOTONCHANNEL(nickname_, channelptr->get_name());
        return(false);
    }
    else if (channelptr->IsClientAnOperator(nickname_) == false)
    {
        output_ += Messages::ERR_CHANOPRIVSNEEDED(nickname_, channelptr->get_name());
        return(false);
    }
    else if (channelptr->IsClientOnChannel(client) == false)
    {
        output_ += Messages::ERR_USERNOTINCHANNEL(client->get_nickname(), channelptr->get_name());
        return(false);
    }
    return(true);
}

bool Client::SameNumbChannelsClientsToKick()
{
    int chan_count = 0;
    std::stringstream channels((params_[0]));
    std::string token;
    while(getline(channels, token, ','))
        chan_count++;
    int user_count = 0;
    std::stringstream user((params_[1]));
    while(getline(user, token, ','))
        user_count++;
    if(user_count == chan_count)
        return(true);
    return(false);
}

void Client::RemoveChannel(Channel* chan)
{
    for(unsigned int i = 0; i < channels_.size(); i++)
    {
        if(channels_[i] == chan)
            channels_.erase(channels_.begin()+i);
    }
}

void Client::ConnectionClosing()
{
    ClientState_ = DISCONNECTED;
    for(unsigned int i = 0; i < channels_.size(); i++)
    {
        channels_[i]->SendMessageToChannel(Messages::RPL_QUIT(nickname_, username_), this);
        channels_[i]->RemoveClientFromChannel(this);
        channels_[i]->RemoveClientAsOperator(this->get_nickname());
    }
    for (unsigned int i = 0; i < chatclients_.size(); i++)
        chatclients_[i]->set_output(Messages::RPL_QUIT(nickname_, username_));
}
