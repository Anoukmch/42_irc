/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/08/03 16:24:46 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{
    std::cout << "Default Constructor" << std::endl;
}

Channel::Channel(std::string name) : name_(name)
{
    std::cout << "Constructor" << std::endl;
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
        name_ =copyop.name_;
        topic_ = copyop.topic_;
        mode_ = copyop.mode_;
    }
    return(*this);
}

Channel::~Channel()
{
    std::cout << "Destructor" << std::endl;
}


// SETTER

void Channel::set_topic(std::string topic)
{
    topic_ = topic;
}

void Channel::set_mode(std::string& mode)
{
    mode_ = mode;
}

void Channel::set_key(std::string key)
{
    key_ = key;
}

void Channel::set_inviteonlyflag(bool status)
{
    is_inviteonly_ = status;
}

// GETTER

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

bool Channel::get_inviteonlyflag()
{
    return(is_inviteonly_);
}

// OTHER

void Channel::AddClientToChannel(Client* c)
{
    clients_.push_back(c);
}

void Channel::RemoveClientFromChannel(Client* c)
{
    std::vector<Client*>::iterator it = clients_.begin();
    while(it != clients_.end())
    {
        if(c == *it) //2 is DISCONNECTED
            it = clients_.erase(it);
        else
            it++;
    }
}