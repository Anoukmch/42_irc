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
# include <string>
# include <sstream>

# include <sys/socket.h>

#include "Server.hpp"
#include "Channel.hpp"
#include "Messages.hpp"

#define RESET       "\033[0m"               /* Reset */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define PURPLE      "\033[35m"              /* Purple */

class Server;

class Client
{

    public:
		Client(int fd, Server* server);
		~Client();

		// SETTER
		void set_nickname(std::string& nickname);
		void set_username(std::string& username);
		void set_output(std::string mess);
		void set_mode(unsigned char c);

		void SetCmdParamsTrailing(std::string buf);
		
		// GETTER
		std::string get_nickname();
		std::string get_username();
		int get_state();
		int get_fd();
		unsigned char get_mode();

		// OTHER
		bool HaveAlreadyChatted(Client* cl);
		void ReceiveCommand();
		void SendData();
		void RemoveChannel(Channel* chan);

		void CheckCommand(std::string buf);
		void AddChatClient(Client* cl);

		void ConnectionClosing();
	
		enum State
		{
			PASS,
			REGISTERED,
			DISCONNECTED
		};

	private:
		Client();
		Client(const Client &copyclass);
		Client& operator= (const Client& copyop);

		int ClientFd_;
		int ClientState_;

		std::string pwd_;
		std::string nickname_;
		std::string username_;

		std::string buffer_;
		std::string output_;

		std::string cmd_;
		std::vector<std::string> params_;
		std::string trailing_;

		std::vector<Channel*> channels_;
		std::vector<Client*> chatclients_;
		Server* server_;
		unsigned char mode_;

		// COMMANDS
		void PassCmd();
		void CapCmd();
		void NickCmd();
		void UserCmd();
		void JoinCmd();
		void PingCmd();
		void ModeCmd();
		void NamesCmd();
		void PartCmd();
		void PrivmsgCmd();
		void InviteCmd();
		void TopicCmd();
		void KickCmd();
		void QuitCmd();
		void NoticeCmd();

		bool IsPossibleToKick(Channel* channel, Client* client);
		bool SameNumbChannelsClientsToKick();
};

#endif
