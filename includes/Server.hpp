/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:42:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/06/27 10:39:51 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_HPP
# define Server_HPP

# include <iostream>
# include <vector>

#define RESET       "\033[0m"               /* Reset */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define PURPLE      "\033[35m"              /* Purple */

#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>

#include "Client.hpp"
#include "Channel.hpp"

#define EVENTS (POLLIN | POLLOUT | POLLERR)
#define MAX_PORT_NUMBER 65535

class Client;
class Channel;
extern bool	server_shutdown;

class Server
{
    public:
		Server(uint16_t port, std::string password);

		// PARSING & SET UP
		bool ValidPort();
		void MainLoop();
		void server_setup();
		
		void acceptConnection();
		void CheckForDisconnections();

		bool IsUniqueNickname(std::string poss_nick);

		void AddChannel(std::string name);
		
		bool CheckPassword(std::string pass);


		// // GETTER
		Channel* GetChannel(std::string name);
		Channel* GetLastChannel();
		Client* GetClient(std::string name);
		const std::string getPassword();

		~Server();
		class SetupError: public std::exception {
			public:
				virtual const char* what() const throw();
		};
	private:
		Server();
		Server(const Server &copyclass);
		Server& operator= (const Server& copyop);

		int serverSocket_;
		uint16_t port_;
		std::string connection_pd_;
		struct sockaddr_in address_;
		std::string mode_;
		std::vector<pollfd>PollStructs_;

		std::vector<Client*>ConnectedClients_;
		std::vector<Channel*>channels_;


};

#endif
