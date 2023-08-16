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

typedef struct s_mode
{
	bool	i;
	bool	t;
	bool	k;
	bool	o;
	bool	l;
}			t_mode;

class Channel
{
    public:
		Channel(std::string name);
		~Channel();

		// SETTER
		void set_topic(std::string topic);
		void set_mode(std::string& mode, std::vector<std::string> params);
		void set_key(std::string key);
		void set_userlimit(int limit);
		void set_inviteonlyflag(bool status);
		void set_topicflag(bool status);

		// GETTER
		std::string get_topic();
		std::string get_mode();
		std::string get_name();
		std::string get_key();
		int			get_userlimit();
		bool		get_inviteonlyflag();
		bool		get_topicflag();
		std::string	GetChannelList();
		int			GetClientNumbers();
		int			GetOperatorNumbers();
		
		// CLIENT OPERATIONS

		void AddClientToChannel(Client* c);
		void RemoveClientFromChannel(Client* c);

		void AddClientAsOperator(std::string nickname);
		void RemoveClientAsOperator(std::string nickname);

		void AddClientAsInvited(std::string nickname);
		void RemoveClientAsInvited(std::string nickname);

		// CHECKER
		bool IsClientOnChannel(Client *c);
		bool IsChannelNotEmpty();
		bool IsClientAnOperator(std::string nickname);
		bool IsClientInvited(std::string nickname);

		// CHANNEL MESSAGE
		void SendMessageToChannel(std::string mess, Client* exclude);

	private:
		Channel();
		Channel(const Channel &copyclass);
		Channel& operator= (const Channel& copyop);

		std::vector<Client*> clients_;
		std::vector<std::string>operator_;
		std::vector<std::string>invited_;

		std::string name_;
		std::string topic_;
		std::string mode_;
		t_mode		chmode_;

		std::string key_;
		int	limit_;
};

#endif
