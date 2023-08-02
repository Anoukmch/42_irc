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

#include "Client.hpp"
#include "Channel.hpp"

#define EVENTS (POLLIN | POLLOUT | POLLERR)

class Client;
class Channel;

class Server
{
    public:
		Server(uint16_t port, std::string password);

		static bool ValidPort();
		static void MainLoop();
		static void server_setup();
		static void acceptConnection();
		static void CheckForDisconnections();


		static bool IsUniqueNickname(std::string poss_nick);
		static void AddChannel(std::string topic);

		// void handleClient();
		// void broadcastMessage();
		// // SETTER
		// void set_topic(std::string& topic);
		// void set_mode(std::string& mode);

		// // GETTER
		static std::string getPassword();
		// std::string get_mode();

		~Server(); //Destructor
		class SetupError: public std::exception {
			public:
				virtual const char* what() const throw();
		};
	private:
		static int serverSocket_;
		uint16_t port_;
		static std::string connection_pd_;
		static struct sockaddr_in address_;
		static std::string mode_;

		static std::vector<pollfd>PollStructs_;
		static std::vector<Client*>ConnectedClients_;

		static std::vector<Channel*>channels_;

		// std::vector<Client> clients_;
		Server(); //Default Constructor
		Server(const Server &copyclass); //Copy Constructor
		Server& operator= (const Server& copyop); //copy assignment operator

};

#endif