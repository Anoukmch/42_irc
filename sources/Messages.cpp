#include "../includes/Messages.hpp"

/***************************
 * ORTHODOX CANONICAL FORM *
 ***************************/

Messages::Messages() {;}

Messages::Messages(const Messages &copyclass)
{
	*this = copyclass;
}

Messages& Messages::operator= (const Messages& copyop)
{
	if (this != &copyop)
	{ ; }
	return(*this);
}

Messages::~Messages(){;}

/***************************
 *     COMMAND REPLIES     *
 ***************************/

std::string Messages::RPL_NICKCHANGE(const std::string& old_nick, const std::string& new_nick, const std::string& user)
{
	std::cout << GREEN << "User changed his nickname from " << old_nick << " to " << new_nick << "!" << RESET << "\n";
	return std::string(":") + old_nick + "!" + user + "@" + HOST + " " + "NICK" + " :" + new_nick + "\r\n";
}

std::string Messages::RPL_CAP()
{
	std::cout << GREEN << "Server-capabilities were sent to client!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " CAP * LS :cap reply...\r\n";
}

std::string Messages::RPL_JOIN(const std::string& nick, const std::string& user, const std::string& channel_name)
{
	std::cout << GREEN << nick << " joined channel " << channel_name << "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " JOIN " + channel_name + " * :" + user + "\r\n";
}

std::string Messages::RPL_JOIN_OR(const std::string& nick, const std::string& user, const std::string& channel_name)
{
	return std::string(":") + nick + "!" + user + "@" + HOST + " JOIN " + channel_name + " * :" + user + "\r\n";
}

std::string Messages::RPL_JOIN_WITHKEY(const std::string& nick, const std::string& user, const std::string& channel_name, std::string key)
{
	std::cout << GREEN << nick << " joined channel " << channel_name << " with key " << key << "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " JOIN " + channel_name + " * :" + user + "\r\n";
}

std::string Messages::RPL_PING(const std::string& nick, const std::string& params_)
{
	std::cout << GREEN << nick << " pinged this server! " << RESET << "\n";
	if (params_ == "0")
		return std::string(":") + HOST + " PONG " + HOST + ": "+ "\r\n";
	return std::string(":") + HOST + " PONG " + HOST + ": " + params_ + "\r\n";
}

std::string Messages::RPL_SETMODECLIENT(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& mode, const std::string& target)
{
	std::cout << GREEN << nick << " adjusted the mode of " << target << " by " << mode << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " MODE " + channel_name + " " + mode + " " + target + "\r\n";
}

std::string Messages::RPL_PART(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string reason)
{
	std::cout << GREEN << nick << " just parted channel " << channel_name + " because of: " + reason + "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " PART " + channel_name + " " + reason + "!" +  "\r\n";
}

std::string Messages::RPL_PART_OR(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string reason)
{
	return std::string(":") + nick + "!" + user + "@" + HOST + " PART " + channel_name + " " + reason + "!" +  "\r\n";
}

std::string	Messages::RPL_PRIVMSG(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg)
{
	std::cout << GREEN << nick << " sent message " << msg + " to " + target +  "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " PRIVMSG " + target + " :" + msg + "\r\n";
}

std::string	Messages::RPL_PRIVMSG_OR(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg)
{
	return std::string(":") + nick + "!" + user + "@" + HOST + " PRIVMSG " + target + " :" + msg + "\r\n";
}

std::string	Messages::RPL_NOTICE(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg)
{
	std::cout << GREEN << nick << " sent notice " << msg + " to " + target +  "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " NOTICE " + target + " :" + msg + "\r\n";
}

std::string Messages::RPL_TOPICCHANGE(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& topic)
{
	std::cout << GREEN << nick << " set the topic of " << channel_name << " to " << topic << "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " TOPIC " + channel_name + " :" + topic + "\r\n";
}

std::string Messages::RPL_KICK_MESSAGE(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target, const std::string reason)
{
	std::cout << GREEN << nick << " kicked " << target << " of " << channel_name << " because: " << reason << "!" << RESET << "\n";
	return std::string(":") +  nick + "!" + user + "@" + HOST + " KICK " + channel_name + " " + target + " :" + reason + "\r\n";
}

std::string Messages::RPL_KICK(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target)
{
	std::cout << GREEN << nick << " kicked " << target << " of " << channel_name << "!" << RESET << "\n";
	return std::string(":") +  nick + "!" + user + "@" + HOST + " KICK " + channel_name + " " + target + "\r\n";
}

std::string Messages::RPL_QUIT(const std::string& nick, const std::string& user)
{
	std::cout << GREEN << nick << " left the server!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " " + "QUIT :" + "Goodbye!" + "\r\n";
}

std::string Messages::RPL_QUIT_MESSAGE(const std::string& nick, const std::string& user, std::string mess)
{
	std::cout << GREEN << nick << " left the server: " << mess << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " " + "QUIT :" + "Goodbye!" + "\r\n";
}

std::string Messages::RPL_BOT(const std::string& nick, const std::string& channel_name, const std::string& msg)
{
	std::cout << GREEN << nick << " requested a bot on " << channel_name << "!" << RESET << "\n";
	std::string partial_msg, total_msg;
	std::istringstream iss(msg);
	while(getline(iss, partial_msg, '\n'))
		total_msg = total_msg + ":BLACKJACKBOT!BOT@" + SERVERNAME + " NOTICE " + channel_name + " :" + partial_msg + "\r\n";
	return total_msg;
}

std::string	Messages::RPL_INVITED(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target)
{
	std::cout << GREEN << target << " has been invited " << " to " << channel_name << " by " << nick << "!" << RESET << "\n";
	return std::string(":") + nick + "!" + user + "@" + HOST + " INVITE " + target + " :" + channel_name + "\r\n";
}


/***************************
 *     NUMERIC REPLIES     *
 ***************************/

std::string Messages::RPL_WELCOME(const std::string& nick, const std::string user)
{
	std::cout << GREEN << "User: " << user << " succesfully registered to the server, using nick " << nick << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 001 " + nick + " :Welcome to the ft_irc network " + nick + "!" + user + "@" + HOST + "\r\n";
}

std::string Messages::RPL_NAMREPLY(const std::string& nick, const std::string& channel_name, const std::string& names_list)
{
	return std::string(":") + SERVERNAME + " 353 " + nick + " = " + channel_name + " :" +  names_list + "\r\n";
}

std::string Messages::RPL_ENDOFNAMES(const std::string& nick, const std::string& channel_name)
{
	std::cout << GREEN << nick << " requested client-list of channel " << channel_name  << "!"<< RESET << "\n";
	return std::string(":")  + SERVERNAME + " 366 " + nick + " " + channel_name + " :END of NAMES list\r\n";
}

std::string Messages::RPL_MODEUSER(const std::string& nick, const std::string& mode)
{
	std::cout << GREEN << nick << " set his mode to " << mode << RESET << "\n";
	return std::string(":") + SERVERNAME + " 221 " + nick + " " + mode + "\r\n";
}

std::string Messages::RPL_CHANNELMODEIS(const std::string& nick, const std::string& channel_name, const std::string& mode)
{
	std::cout << GREEN << nick << " requested " << channel_name << "'s mode: " << mode << RESET << "\n";
	return std::string(":") + SERVERNAME + " 324 " + nick + " " + channel_name + " " + mode + "\r\n";
}

std::string Messages::RPL_SETMODECHANNEL(const std::string& nick, const std::string& channel_name, const std::string& mode)
{
	std::cout << GREEN << nick << " set " << channel_name << "'s mode to " << mode << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 324 " + nick + " " + channel_name + " " + mode + "\r\n";
}

std::string Messages::RPL_YOUREOPER(const std::string& nick, const std::string& target)
{
	std::cout << GREEN << nick << " make " << target << " a server operator!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 381 " + "PASS :You are now an IRC operator\r\n";
}

std::string	Messages::RPL_INVITING(const std::string& nick, const std::string& channel_name, const std::string& target)
{
	std::cout << GREEN << nick << " invited " << target << " to " << channel_name << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 341 " + nick + " " + target + " " + channel_name + "\r\n";
}

std::string Messages::RPL_NOTOPIC(const std::string& nick, const std::string& channel_name)
{
	std::cout << GREEN << nick << " requested the topic of " << channel_name << " but no topic was set!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 331 " + nick + " " + channel_name + " :No topic is set\r\n";
}

std::string Messages::RPL_TOPIC(const std::string& nick, const std::string& channel_name, const std::string& topic)
{
	std::cout << GREEN << nick << " requested the topic of " << channel_name << ": " << topic << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 332 " + nick + " " + channel_name + " :" + topic + "\r\n";
}

/***************************
 *      ERROR REPLIES      *
 ***************************/

std::string Messages::ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " tried joining channel " << channel_name << " but used the wrong key!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 475 " + channel_name + " :Cannot join channel (+k)\r\n";
}

std::string Messages::ERR_NEEDMOREPARAMS(const std::string& command)
{
	std::cout << RED << command << ": Change the amount of parameters to execute command: " << command << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 461 " + command + " :Not enough parameters\r\n";
}

std::string Messages::ERR_PASSWDMISMATCH()
{
	std::cout << RED << "User provided invalid password!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 464 " + "PASS" + " :Password incorrect\r\n";
}

std::string Messages::ERR_ALREADYREGISTRED()
{
	std::cout << RED << "User tried to re-authenticate!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 462 " ":Already registered in\r\n";
}

std::string Messages::ERR_NICKNAMEINUSE(const std::string& nick)
{
	std::cout << RED << "User tried claiming nickname already in use!" << RESET << "\n";
	return std::string(":") + SERVERNAME " 433 " + nick + " :Nickname is already in use\r\n";
}

std::string Messages::ERR_NONICKNAMEGIVEN()
{
	std::cout << RED << "User did not provide nickname!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 431 " + "NICK" + " :Nickname not given\r\n";
}

std::string Messages::ERR_NOTREGISTERED(const std::string& command)
{
	std::cout << RED << "User tried executing command " << command << " but was not registred or authenticated!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 451 " + ":You have not registered\r\n";
}

std::string Messages::ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << "User tried accessing channel " << channel_name << " but it did not exist" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 403 " +  nick + " " + channel_name + " : No such channel\r\n";
}

std::string Messages::ERR_ERRONEUSNICKNAME(const std::string& nick, const std::string& wrong_nick)
{
	std::cout << RED << nick << " tried using invalid nick: " << wrong_nick  << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 432 " + nick + " " + wrong_nick + " :Erroneous nickname\r\n";
}

std::string Messages::ERR_USERSDONTMATCH(const std::string& nick, const std::string& target)
{
	std::cout << RED << nick << " tried changing mode for " << target << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 502 " + nick + " : Cannot change mode for other users\r\n";
}

std::string Messages::ERR_UNKNOWNCOMMAND(const std::string& nick, const std::string& command)
{
	std::cout << RED "User tried executing unknown command: " << command << "!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 421 " + nick + " " + command + " :Unknown command\r\n";
}

std::string Messages::ERR_NOSUCHNICK(const std::string& nick, const std::string channel_name)
{
	std::cout << RED << "User "  << nick << " tried accessing " << channel_name << " but it did not exist!" << RESET << "\n";
	return std::string(":") + SERVERNAME " 401 " + nick + " " + channel_name + " :No such nick/channel\r\n";
}

std::string Messages::ERR_NOSUCHNICK_NICKONLY(const std::string& nick)
{
	std::cout << RED << "No user with nickname " << nick << " exists!" << RESET << "\n";
	return std::string(":") + SERVERNAME " 401 " + nick + " :No such nick/channel\r\n";
}

std::string Messages::ERR_CANNOTSENDTOCHAN(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " tried sending message to " << channel_name << " but was not registered to it!" << RESET << "\n";
	return std::string(":") + SERVERNAME +  " 404 " + nick + " " + channel_name + " :Cannot send to channel\r\n";
}

std::string Messages::ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " tried changing mode for " << channel_name << " but did not have op permissions!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 482 " + nick + " " + channel_name + " :You're not channel operator \r\n";
}

std::string Messages::ERR_NOPRIVS(const std::string& nick, const std::string& command)
{
	std::cout << RED << nick << "tried executing OP Command " << command << " but is not Operator!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 481 " + command + " :Permission Denied- You're not an IRC operator\r\n";
}

std::string Messages::ERR_UMODEUNKNOWNFLAG(const std::string& nick)
{
	std::cout << RED << "User used invalid mode flags " << RESET << "\n";
	return std::string(":") + SERVERNAME + " 501 " + nick + " :Unknown MODE flags\r\n";
}

std::string Messages::ERR_USERONCHANNEL(const std::string& nick, const std::string& channel_name, const std::string& target)
{
	std::cout << RED << nick << " inviting " << target << " to " << channel_name << " but he was already on channel!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 443 " + nick + " " + channel_name + " :Is already on channel \r\n";
}

std::string Messages::ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " is not in"  << channel_name << "!" << RESET << "\n";
	return std::string(":") + "441 " + SERVERNAME + " " + nick + " " + channel_name + " : is not on channel \r\n";
}

std::string Messages::ERR_NOTONCHANNEL(const std::string nick, const std::string channel_name)
{
	std::cout << RED << nick << "tried using command on channel he isn't registered to!" << RESET << "\n";
	return std::string(":") + SERVERNAME + + " 401 " + nick + " " + channel_name + " : You're not on that channel \r\n";
}

std::string Messages::ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " tried joining " << channel_name << " but he was not invited!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 473 " + nick + " " + channel_name + " : Cannot join channel (+i) - you must be invited\r\n";
}

std::string Messages::ERR_CHANNELISFULL(const std::string& nick, const std::string& channel_name)
{
	std::cout << RED << nick << " tried joining " << channel_name << " but channel is full!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 471 " + nick + " " + channel_name + " : Cannot join channel (+l) - channel is full55\r\n";
}

std::string Messages::ERR_NOSUCHSERVER(const std::string& nick, const std::string& token)
{
	std::cout << RED << nick << " executes the PING command with the name of a non-existent server!" << RESET << "\n";
	return std::string(":") + SERVERNAME + " 402 " + token + " :No such server\r\n";
}

std::string Messages::ERR_NOOPERHOST(const std::string& nick)
{
	std::cout << RED << nick << " Wrong host for oper command!" << RESET << "\n";
	return std::string(":") + nick + ": " + nick + " : No O-lines for your host\r\n";
}
