/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/16 13:17:04 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

/***************************
 * ORTHODOX CANONICAL FORM *
 ***************************/

Channel::Channel()
{
    std::cout << "Default Constructor" << std::endl;
}

Channel::Channel(const Channel &copyclass)
{
    std::cout << "Copy Constructor" << std::endl;
    *this = copyclass;
}

Channel& Channel::operator= (const Channel& copyop)
{
    std::cout << "Copy Assignment Operator" << std::endl;
    if(this != &copyop)
    {
        name_ = copyop.name_;
        topic_ = copyop.topic_;
        mode_ = copyop.mode_;
    }
    return(*this);
}

Channel::~Channel()
{
    // std::cout << "Destructor" << std::endl;
}

/***************************
 * CONSTRUCTOR WITH PARAMS *
 ***************************/

Channel::Channel(std::string name) : name_(name), limit_(100)
{
	chmode_.i = false;
	chmode_.t = false;
	chmode_.k = false;
	chmode_.o = true;
	chmode_.l = false;
	mode_ = "+";
    // std::cout << "Constructor" << std::endl;
}

/***************************
 *          SETTER         *
 ***************************/

void Channel::set_topic(std::string topic)
{
    topic_ = topic;
}

void Channel::set_mode(std::string& mode, std::vector<std::string> params)
{
	if (mode[1] == 'i')
	{
		if (mode[0] == '+')
			chmode_.i = true;
		else if (mode[0] == '-')
			chmode_.i = false;
	}
	else if (mode[1] == 't')
	{
		if (mode[0] == '+')
			chmode_.t = true;
		else if (mode[0] == '-')
			chmode_.t = false;
	}
	else if (mode[1] == 'k')
	{
		if (mode[0] == '+')
		{
			chmode_.k = true;
			set_key(params[2]);
		}
		else if (mode[0] == '-')
		{
			chmode_.k = false;
			key_ = "";
		}
	}
	else if (mode[1] == 'o')
	{
		if (mode[0] == '+')
		{
			chmode_.o = true;
			AddClientAsOperator(params[2]);
		}
		else if (mode[0] == '-')
		{
			chmode_.o = false;
			RemoveClientAsOperator(params[2]);
		}
	}
	else if (mode[1] == 'l')
	{
		if (mode[0] == '+')
		{
			const char *x = params[2].c_str();
			limit_ = atoi(x);
			chmode_.l = true;
		}
		else if (mode[0] == '-')
		{
			limit_ = 100;
			chmode_.l = false;
		}
	}
	mode_.clear();
	mode_ = "+";
	if (chmode_.i == true)
		mode_.append("i");
	if (chmode_.t == true)
		mode_.append("t");
	if (chmode_.k == true)
		mode_.append("k");
	if (chmode_.o == true)
		mode_.append("o");
	if (chmode_.l == true)
		mode_.append("l");
}

void Channel::set_key(std::string key)
{
	key_ = key;
}

void Channel::set_userlimit(int limit)
{
	limit_ = limit;
}

void Channel::set_inviteonlyflag(bool status)
{
    chmode_.i = status;
}

void Channel::set_topicflag(bool status)
{
    chmode_.t = status;
}

/***************************
 *          GETTER         *
 ***************************/

std::string Channel::get_name()
{
    return(name_);
}

std::string Channel::get_topic()
{
    return(topic_);
}

std::string Channel::get_mode()
{
    return(mode_);
}

std::string Channel::get_key()
{
    return(key_);
}

int Channel::get_userlimit()
{
    return(limit_);
}

bool Channel::get_inviteonlyflag()
{
    return(chmode_.i);
}

bool Channel::get_topicflag()
{
    return(chmode_.t);
}

int	Channel::GetClientNumbers()
{
	return (clients_.size());
}

int	Channel::GetOperatorNumbers()
{
	return (operator_.size());
}

std::string	Channel::GetChannelList()
{
	std::string channel_list;
	std::vector<Client*>::iterator it = clients_.begin();
	while(it != clients_.end())
	{
		std::vector<std::string>::iterator op = operator_.begin();
		while(op != operator_.end())
		{
			if (*op == (*it)->get_nickname())
				channel_list += "@";
			op++;
		}
		channel_list += (*it)->get_nickname() + " ";
		it++;
	}
	channel_list += "\r\n";
	return(channel_list);
}

/***************************
 *    CLIENT OPERATIONS    *
 ***************************/

void Channel::AddClientToChannel(Client* c)
{
    clients_.push_back(c);
}

void Channel::RemoveClientFromChannel(Client* c)
{
    std::vector<Client*>::iterator it = clients_.begin();
    while(it != clients_.end())
    {
        if(c == *it)
            it = clients_.erase(it);
        else
            it++;
    }
}

void Channel::AddClientAsOperator(std::string nickname)
{
    operator_.push_back(nickname);
}

void Channel::RemoveClientAsOperator(std::string nickname)
{
    std::vector<std::string>::iterator it = operator_.begin();
    while(it != operator_.end())
    {
        if(nickname == *it)
            it = operator_.erase(it);
        else
            it++;
    }
}

void Channel::AddClientAsInvited(std::string nickname)
{
    invited_.push_back(nickname);
}


void Channel::RemoveClientAsInvited(std::string nickname)
{
    std::vector<std::string>::iterator it = invited_.begin();
    while(it != invited_.end())
    {
        if(nickname == *it)
            it = operator_.erase(it);
        else
            it++;
    }
}

/***************************
 *         CHECKER         *
 ***************************/

bool Channel::IsClientOnChannel(Client *c)
{
    std::vector<Client*>::iterator it = clients_.begin();
    while(it != clients_.end())
    {
        if(c == *it)
            return(true);
        it++;
    }
    return(false);
}

bool Channel::IsClientAnOperator(std::string nickname)
{
    std::vector<std::string>::iterator it = operator_.begin();
    while(it != operator_.end())
    {
        if(nickname == *it)
            return(true);
        it++;
    }
    return(false);
}

bool Channel::IsClientInvited(std::string nickname)
{
    std::vector<std::string>::iterator it = invited_.begin();
    while(it != invited_.end())
    {
        if(nickname == *it)
            return(true);
        it++;
    }
    return(false);
}

bool Channel::IsChannelNotEmpty()
{
    if(clients_.empty() == true)
        return(false);
    return(true);
}

/***************************
 *     CHANNEL MESSAGE     *
 ***************************/

void Channel::SendMessageToChannel(std::string mess, Client* exclude)
{
    for(unsigned int i = 0; i < clients_.size(); i++)
    {
        if(clients_[i] != exclude)
            clients_[i]->set_output(mess);
    }
}
