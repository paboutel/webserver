/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 16:38:56 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ConfigParser.hpp"
# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>

class Servers {
	public:
		/* ------------------------- socket structure utils ------------------------- */
		struct socket_t {
			int			sockfd;
			sockaddr_in	address;
			int			opt;
			int			addrLen;
			std::string ip;
			std::string port;
		};

		typedef std::map<int, socket_t> sock_type;
		
	private:
		sock_type	_sockIpPort;

		void		_listenConnection(void);
		void		_createNewServer(std::string ip, std::string port);
		
	public:
		/* ------------------------------- constructor ------------------------------ */
		Servers(ConfigParser &confFile);
		~Servers(void);

		/* --------------------------------- methods -------------------------------- */
		sock_type::iterator	getSocketByFd(int fd);
		int					acceptSocket(socket_t sock);
		sock_type			&getSockIpPort(void);
		std::string			findIpByFd(int fd);
		std::string			findPortByFd(int fd);
		std::string			getClientIp(socket_t &sock, int clientFd);

		/* ------------------------------- error class ------------------------------ */
		class ServersError: public std::exception {
			private:
				std::string	_error;
			public:
				ServersError(std::string error) : _error(error) {};
				virtual const char *what() const throw() {
					return (_error.c_str());
				}
				virtual ~ServersError(void) throw() {};
		};
};
