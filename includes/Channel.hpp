/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:42:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/06/27 10:39:51 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>

#define RESET       "\033[0m"               /* Reset */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define PURPLE      "\033[35m"              /* Purple */

# include "Client.hpp"
# include "Server.hpp"

class Client;

class Channel
{
    public:
		Channel(std::string name);
		~Channel(); //Destructor

		// SETTER
		void set_topic(std::string topic);
		void set_mode(std::string& mode);
		void set_key(std::string key);
		void set_inviteonlyflag(bool status);

		// GETTER
		std::string get_topic();
		std::string get_mode();
		std::string get_name();
		std::string get_key();
		bool get_inviteonlyflag();

		void SendMessageToChannel(std::string mess, Client* exclude);
		void AddClientToChannel(Client* c);
		void RemoveClientFromChannel(Client* c);
		bool IsClientOnChannel(Client *c);
		bool IsChannelNotEmpty();
	private:
		Channel(); //Default Constructor
		Channel(const Channel &copyclass); //Copy Constructor
		Channel& operator= (const Channel& copyop); //copy assignment operator

		std::vector<Client*> clients_;

		std::string name_;
		std::string topic_;
		std::string mode_;
		
		std::string key_; //key, if empty none
		bool is_inviteonly_;
		std::string channeloperator_; //name, if empty none
};

#endif