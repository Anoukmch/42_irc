/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatheis <jmatheis@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:42:14 by jmatheis          #+#    #+#             */
/*   Updated: 2023/06/27 10:39:51 by jmatheis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>

# include <sys/socket.h>

#define RESET       "\033[0m"               /* Reset */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define PURPLE      "\033[35m"              /* Purple */

class Client
{
    public:
		Client(); //Default Constructor
		Client(int fd);
		Client(const Client &copyclass); //Copy Constructor
		Client& operator= (const Client& copyop); //copy assignment operator
		~Client(); //Destructor

		// SETTER
		void set_nickname(std::string& nickname);
		void set_username(std::string& username);

		// GETTER
		std::string get_nickname();
		std::string get_username();

		// OTHER
		void ConnectionClosing();
		void ReceiveCommand();
		void SendData();

	private:
		int ClientFd_;
		std::string nickname_;
		std::string username_;
};

#endif