/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:23:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/07/26 14:50:00 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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

void Channel::set_topic(std::string& topic)
{
    topic_ = topic;
}
void Channel::set_mode(std::string& mode)
{
    mode_ = mode;
}

// GETTER

std::string Channel::get_topic()
{
    return(topic_);
}

std::string Channel::get_mode()
{
    return(mode_);
}

