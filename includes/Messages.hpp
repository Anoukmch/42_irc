#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#define RESET       "\033[0m"               /* Reset */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define PURPLE      "\033[35m"              /* Purple */

#include <iostream>
#include <sstream>

#define HOST "localhost"
#define SERVERNAME "irc2"


class Messages
{
    public:
		// COMMAND MESSAGES
		static std::string RPL_NICKCHANGE(const std::string& old_nick, const std::string& new_nick, const std::string& user);
		static std::string RPL_CAP();
		static std::string RPL_JOIN(const std::string& nick, const std::string& user, const std::string& channel_name);
		static std::string RPL_JOIN_OR(const std::string& nick, const std::string& user, const std::string& channel_name);
		static std::string RPL_JOIN_WITHKEY(const std::string& nick, const std::string& user, const std::string& channel_name, std::string key);

		static std::string RPL_PING(const std::string& nick, const std::string& params_);
		static std::string RPL_SETMODECLIENT(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& mode, const std::string& target);
		static std::string RPL_PART(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string reason);
		static std::string RPL_PART_OR(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string reason);
		static std::string RPL_PRIVMSG(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg);
		static std::string RPL_PRIVMSG_OR(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg);
		static std::string RPL_NOTICE(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg);
		static std::string RPL_TOPICCHANGE(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& topic);
		static std::string RPL_KICK(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target);
		static std::string RPL_KICK_MESSAGE(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target, const std::string reason);
		static std::string RPL_QUIT(const std::string& nick, const std::string& user);
		static std::string RPL_QUIT_MESSAGE(const std::string& nick, const std::string& user, std::string mess);
		static std::string RPL_BOT(const std::string& nick, const std::string& channel_name, const std::string& msg);
		static std::string RPL_INVITED(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target);

		// NUMERIC MESSAGES
		static std::string RPL_WELCOME(const std::string& nick, const std::string user);
		static std::string RPL_NAMREPLY(const std::string& nick, const std::string& channel_name, const std::string& names_list);
		static std::string RPL_ENDOFNAMES(const std::string& nick, const std::string& channel_name);
		static std::string RPL_MODEUSER(const std::string& nick, const std::string& mode);
		static std::string RPL_CHANNELMODEIS(const std::string& nick, const std::string& channel_name, const std::string& mode);
		static std::string RPL_SETMODECHANNEL(const std::string& nick, const std::string& channel_name, const std::string& mode);
		static std::string RPL_YOUREOPER(const std::string& nick, const std::string& target);
		static std::string RPL_INVITING(const std::string& nick, const std::string& channel_name, const std::string& target);
		static std::string RPL_NOTOPIC(const std::string& nick, const std::string& channel_name);
		static std::string RPL_TOPIC(const std::string& nick, const std::string& channel_name, const std::string& topic);

		// ERROR MESSAGES
		static std::string ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel_name);
		static std::string ERR_NEEDMOREPARAMS(const std::string& command);
		static std::string ERR_PASSWDMISMATCH();
		static std::string ERR_ALREADYREGISTRED() ;
		static std::string ERR_NICKNAMEINUSE(const std::string& nick);
		static std::string ERR_NONICKNAMEGIVEN();
		static std::string ERR_NOTREGISTERED(const std::string& command);
		static std::string ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name);
		static std::string ERR_ERRONEUSNICKNAME(const std::string& nick, const std::string& wrong_nick);
		static std::string ERR_USERSDONTMATCH(const std::string& nick, const std::string& target);
		static std::string ERR_UNKNOWNCOMMAND(const std::string& nick, const std::string& command);
		static std::string ERR_NOSUCHNICK_NICKONLY(const std::string& nick);
		static std::string ERR_NOSUCHNICK(const std::string& nick, const std::string channel_name);
		static std::string ERR_CANNOTSENDTOCHAN(const std::string& nick, const std::string& channel_name);
		static std::string ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel_name);
		static std::string ERR_NOPRIVS(const std::string& nick, const std::string& command);
		static std::string ERR_UMODEUNKNOWNFLAG(const std::string& nick);
		static std::string ERR_USERONCHANNEL(const std::string& nick, const std::string& channel_name, const std::string& target);
		static std::string ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& channel_name);
		static std::string ERR_NOTONCHANNEL(const std::string nick, const std::string channel_name);
		static std::string ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel_name);
		static std::string ERR_CHANNELISFULL(const std::string& nick, const std::string& channel_name);
		static std::string ERR_NOSUCHSERVER(const std::string& nick, const std::string& token);
		static std::string ERR_NOOPERHOST(const std::string& nick);


	private:
		Messages();
		Messages(const Messages &copyclass);
		Messages& operator= (const Messages& copyop);
		~Messages();
};

#endif
