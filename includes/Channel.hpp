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

class Channel
{
    public:
		Channel(); //Default Constructor
		Channel(const Channel &copyclass); //Copy Constructor
		Channel& operator= (const Channel& copyop); //copy assignment operator
		~Channel(); //Destructor

		// SETTER
		void set_topic(std::string& topic);
		void set_mode(std::string& mode);

		// GETTER
		std::string get_topic();
		std::string get_mode();

	private:
		std::string topic_;
		std::string mode_;
};

#endif