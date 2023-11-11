/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:56:52 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/31 11:43:44 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Servers.hpp"
# include <sys/epoll.h>

class IOpoll {
	private:
		int epollfd;
		struct epoll_event ev;
		struct epoll_event *events;
		
	public:
		IOpoll(Servers &servers);
		~IOpoll();

		int	getEpollfd(void) const;
		epoll_event	getEvent(void) const;
		epoll_event *getEvents(void) const;

		void	addFd(int fd);

		class IOpollError: public std::exception {
			private:
				std::string	_error;
			public:
				IOpollError(const std::string &error)
				: _error(error) {}

				virtual ~IOpollError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
};
