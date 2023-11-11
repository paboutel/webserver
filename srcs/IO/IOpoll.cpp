/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:57:26 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/10 11:50:38 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOpoll.hpp"
#include "globalDefine.hpp"
#include <cstdlib>
#include <stdio.h>

IOpoll::IOpoll(Servers &servers) {
	this->ev.events = EPOLLIN | EPOLLHUP | EPOLLERR;
	this->events = new epoll_event[QUE_SIZE];
	this->epollfd = epoll_create1(0);
	this->ev.data.ptr = NULL;
	
	if (this->epollfd < 0)
		throw IOpollError("Epoll creation failure");

	Servers::sock_type serv = servers.getSockIpPort();
	for (Servers::sock_type::iterator it = serv.begin(); it != serv.end(); it++) {
		ev.data.fd = it->first;
		if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, it->first, &this->ev)) {
			close(this->epollfd);
			throw IOpollError("failed to add server fd to the watchlist");
		}
	}
}

IOpoll::~IOpoll(void) {
	close(this->epollfd);
	delete []events;
}

/* -------------------------------------------------------------------------- */
/*                                  Accessor                                  */
/* -------------------------------------------------------------------------- */

int	IOpoll::getEpollfd(void) const{
	return (this->epollfd);
}

epoll_event	IOpoll::getEvent(void) const{
	return (this->ev);
}

epoll_event	*IOpoll::getEvents(void) const{
	return (this->events);
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

void	IOpoll::addFd(int fd) {
	ev.events = EPOLLIN | EPOLLHUP | EPOLLERR;
	ev.data.fd = fd;
	if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &this->ev)) {
		close(this->epollfd);
		throw IOpollError("failed to add server fd to the watchlist");
	}
}

