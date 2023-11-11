/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/13 11:13:29 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "utils.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "main.hpp"
#include "colors.hpp"
#include "globalDefine.hpp"
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <ostream>
#include "CgiHandler.hpp"

bool g_exit = false;

ConfigParser::Server    selectServ(std::string ip, std::string port, std::string hostName, ConfigParser::data_type vecServs)
{
   	ConfigParser::data_type::iterator   firstOccu = vecServs.end();
	
   	for (ConfigParser::data_type::iterator it = vecServs.begin(); it != vecServs.end(); it++)
   	{
		if (!it->listen.count(ip) || !it->listen[ip].count(port))
		   continue ;
		if (it->server_name.count(hostName))
			return (*it);
		else if (firstOccu == vecServs.end())
			firstOccu = it;
   	}
	if (firstOccu == vecServs.end())
	{
		std::cout << "ip : " << ip << std::endl
				  << "port : " << port << std::endl
				  << "hostname : " << hostName << std::endl;
		throw InvalidHost("Error: invalid host");
	}
   	return (*firstOccu);
}

ConfigParser::Location	getEnvFromTarget(std::string target, ConfigParser::Server serv)
{
	std::vector<std::string>	splitedTarget = split(target, "/");
	ConfigParser::Location		res;
	
	for (std::vector<std::string>::reverse_iterator	rit = splitedTarget.rbegin(); rit != splitedTarget.rend(); rit++)
	{
		std::string	currLoc;
		for (std::vector<std::string>::iterator it = splitedTarget.begin(); std::reverse_iterator< std::vector<std::string>::iterator >(it) != rit; it++)
			currLoc += "/" + *it;
		if (serv.location.count(currLoc))
		{
			res.uniqKey.insert(std::make_pair("_rootToDel_", std::vector<std::string> (1, currLoc)));
			res.insert(serv.location[currLoc]);
		}
	}
	res.uniqKey.insert(std::make_pair("_rootToDel_", std::vector<std::string> (1, "")));
	res.insert(serv.location["/"]);
	return (res);
}

void	handle_sig(int sig)
{
	if (sig == SIGINT)
		g_exit = true;
}

void	closeRequest(std::map<int, Request> requests)
{
	for (std::map<int, Request>::iterator it = requests.begin(); it != requests.end(); it++)
		close(it->first);
}

void closeAndErase(int &fdTriggered, std::map<int, int> &clientList, std::map<int, Request> &requests, std::map<int, Response> &responses)
{
	close(fdTriggered);
	if (responses.count(fdTriggered))
		responses.erase(responses.find(fdTriggered));
	requests.erase(requests.find(fdTriggered));
	clientList.erase(clientList.find(fdTriggered));
}

int main(int ac, char **av, char **sysEnv)
{	
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments for WebServ" << std::endl;
		exit (EXIT_FAILURE);
	}
	std::map<int, int>			clientList;
	std::map<int, Request>	 	requests;
	std::map<int, Response> 	responses;
	
	try {	
	/* --------------------------------- Parsing -------------------------------- */
		ConfigParser	configServers(av[1]);
		Servers 		serverList(configServers);
		IOpoll			epoll(serverList);

		std::signal(SIGINT, handle_sig);


	/* ----------------------------- Server loop ---------------------------- */
		std::cout << "init " << C_GREEN << "Success" << C_RESET << ", server is running" << std::endl;
		while (!g_exit) 
		{
			try 
			{
				if (g_exit)
					break ;
				int numberFdReady = epoll_wait(epoll.getEpollfd(), epoll.getEvents(), QUE_SIZE, -1);
				if (g_exit)
					break ;
				if (numberFdReady == -1)
					break ;
				for (int index = 0; index != numberFdReady; index++)
				{
					int	fdTriggered = epoll.getEvents()[index].data.fd;
					Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(fdTriggered);
					if (sockTarget != serverList.getSockIpPort().end())
					{
						int clientSocket = 0;
						
						try {
							clientSocket = serverList.acceptSocket(sockTarget->second);
							epoll.addFd(clientSocket);
							clientList.insert(std::make_pair(clientSocket, fdTriggered));
							requests.insert(std::make_pair(clientSocket, Request(clientSocket)));
						} catch (std::exception &e) {
							std::cout << e.what() << std::endl;
							if (clientSocket)
								close(clientSocket);
						}
					}
					else
					{
						std::map<int, int>::iterator		pairContacted = clientList.find(fdTriggered);
						std::map<int, Request>::iterator	req = requests.find(fdTriggered);
						int									event = epoll.getEvents()[index].events;

						try {
							if (event & EPOLLHUP || event & EPOLLERR)
							{
								closeAndErase(fdTriggered, clientList, requests, responses);
								continue ;
							}
							if (event & EPOLLIN)
							{
								// s'il reste à lire on continue. Si la requête est invalide/incomplète recv throw, si la requête est complète on exec le reste du process
								if (!req->second.rec())
								{
									epoll.getEvents()[index].events = EPOLLIN | EPOLLHUP | EPOLLERR;
									continue ;
								}
								ConfigParser::Server	server = selectServ(serverList.findIpByFd(pairContacted->second), serverList.findPortByFd(pairContacted->second), req->second.getData()["Host"][0], configServers.getData());
								ConfigParser::Location	env = getEnvFromTarget(req->second.getTarget(), server);
								responses.insert(std::make_pair(fdTriggered, Response (env, req->second, serverList.getClientIp(sockTarget->second, pairContacted->first), sysEnv)));
								responses[fdTriggered].execute();
								responses[fdTriggered].constructData();
							}
							if (!responses.count(fdTriggered))
								closeAndErase(fdTriggered, clientList, requests, responses);
							if (responses.find(fdTriggered)->second.sendData(pairContacted->first))
								closeAndErase(fdTriggered, clientList, requests, responses);
							else
								epoll.getEvents()[index].events = EPOLLOUT | EPOLLHUP | EPOLLERR; // set EPOLLOUT au cas où on ait besoin de revenir pour finir l'envoi
						}
						catch (CgiHandler::CgiHandlerError &e) { break; }
						catch (std::exception &e) { 
							std::cerr << C_ORANGE << "Server is listening: " << e.what() << C_RESET << std::endl;
							closeAndErase(fdTriggered, clientList, requests, responses);
						}
					}
				}
			} 
			catch (std::exception &e) { std::cerr << C_ORANGE << "Server is listening: " << e.what() << C_RESET << std::endl; }
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "init " << C_RED << "Failed" << C_RESET << std::endl
				  << "error : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	closeRequest(requests);
	std::cout << std::endl << C_PURPLE << "GoodBye" << C_RESET << std::endl;
	return (EXIT_SUCCESS);
}
