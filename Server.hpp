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

#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
    public:
		Server(uint16_t port, std::string password);

		void server_setup();
		void acceptConnection();
		void handleClient();
		void broadcastMessage();
		// // SETTER
		// void set_topic(std::string& topic);
		// void set_mode(std::string& mode);

		// // GETTER
		// std::string get_topic();
		// std::string get_mode();

		~Server(); //Destructor
		class SetupError: public std::exception {
			public:
				virtual const char* what() const throw();
		};
	private:
		int server_fd_;
		int server_sock_;
		uint16_t port_;
		std::string connection_pd_;
		struct sockaddr_in address_;
		std::string mode_;
		std::vector<Client> clients_;
		std::vector<Channel> channels_;
		Server(); //Default Constructor
		Server(const Server &copyclass); //Copy Constructor
		Server& operator= (const Server& copyop); //copy assignment operator

};

#endif