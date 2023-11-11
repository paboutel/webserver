/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/06 14:40:48 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"
#include "globalDefine.hpp"
#include "colors.hpp"
#include <cstdlib>
#include <pthread.h>

void	Servers::_createNewServer(std::string ip, std::string port)
{
	socket_t	socketInfo;
	int			sockfd;
	
	socketInfo.ip = ip;
	socketInfo.port = port;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	socketInfo.sockfd = sockfd;
	if (!sockfd)
		throw ServersError("Socket creation failed");
	socketInfo.opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketInfo.opt, sizeof(socketInfo.opt)) < 0) {
	    close(sockfd);
        throw ServersError("Configuration failed");
	}
	
	socketInfo.address.sin_family = AF_INET;
	socketInfo.address.sin_addr.s_addr = inet_addr(ip.c_str());
	socketInfo.address.sin_port = htons(atoi(port.c_str()));
	socketInfo.addrLen = sizeof(socketInfo.address);
	
	if (bind(sockfd, (struct sockaddr*)&socketInfo.address, sizeof(socketInfo.address)) < 0) {
	    close(sockfd);
		throw ServersError("binding socket failed");
	}
	_sockIpPort.insert(std::make_pair(sockfd, socketInfo));
}

void    Servers::_listenConnection(void)
{
    for (Servers::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
    {        
        if (listen(it->first, QUE_SIZE) < 0)
            throw ServersError("listen failed");
    }
}

/*
Go through all the server config file to create new server for each pair host/port
*/
Servers::Servers(ConfigParser &confFile) {
	ConfigParser::data_type conf = confFile.getData();

	std::set< std::pair<std::string, std::string> >	bindedSocketList;

	try
	{
		for (ConfigParser::data_type::iterator itconf = conf.begin(); itconf != conf.end(); itconf++)
		{
			for (ConfigParser::Server::listen_type::iterator itListen = itconf->listen.begin(); itListen != itconf->listen.end(); itListen++)
			{
				for (std::set<std::string>::iterator itSet = itListen->second.begin(); itSet != itListen->second.end(); itSet++)
				{
					if (!bindedSocketList.count(std::make_pair(itListen->first, *itSet)))
					{
						_createNewServer(itListen->first, *itSet);
						bindedSocketList.insert(std::make_pair(itListen->first, *itSet));
					}
					std::cout << "\rServer " << "0" << " listening on " << itListen->first << ":" << *itSet << " ...        ";
					std::cout.flush();
					usleep(150000);
				}
			}
			std::cout << "\rServer " << itconf - conf.begin() << C_GREEN << " UP                                       " << C_RESET << std::endl;
		}
		_listenConnection();
	}
	catch (std::exception &e)
	{
		for (Servers::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
			close(it->first);
		throw ServersError(e.what());
	}
}

Servers::~Servers(void)
{
	for (Servers::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
		close(it->first);
}

Servers::sock_type::iterator Servers::getSocketByFd(int fd)
{
    Servers::sock_type::iterator it = _sockIpPort.begin();
	
    for (; it != _sockIpPort.end(); it++)
    {
        if (it->first == fd)
            return (it);
    }
    return (it);
}

int	Servers::acceptSocket(Servers::socket_t sock)
{
	int newSocket = accept4(sock.sockfd, (struct sockaddr *)&sock.address, (socklen_t *)&sock.addrLen, SOCK_NONBLOCK);
	if (newSocket < 0)
		throw ServersError("Accept failed");
	return (newSocket);
}

Servers::sock_type	&Servers::getSockIpPort()
{
	return (_sockIpPort);
}

std::string	Servers::findIpByFd(int fd)
{ return (_sockIpPort[fd].ip); }

std::string	Servers::findPortByFd(int fd)
{ return (_sockIpPort[fd].port); }

std::string Servers::getClientIp(Servers::socket_t &sock, int clientFd)
{
	sockaddr_in tmp = sock.address;
	socklen_t len = sizeof(tmp);
	if (getpeername(clientFd, (struct sockaddr *)&(tmp), &len) == -1)
		throw ServersError("can't retrieve the ip of the client (getpeername failed)");
	return (std::string(inet_ntoa(tmp.sin_addr)));
}
