/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 22:46:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/05 15:38:22 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
#include "Response.hpp"

std::vector<char>	createIndexDir(std::vector<std::string> listOfFiles, std::string path)
{
	std::string	html;
	html.append(Response::_css);
	html.append("<style>\
		a { font-size: 3em;\
			color:bd93f9;\
		}\
		li {list-style-type:none;}\
		h1 {font-size:3em;\
			text-decoration: underline;\
		}\
	</style>");
	html.append("<center><h1>Index of \"" + path + "\" : </h1></center>");
	html.append("<ul>");
	if (*path.rbegin() != '/')
		path.push_back('/');
	for (std::vector<std::string>::iterator it = listOfFiles.begin(); it != listOfFiles.end(); it++)
		html.append("<center><li><a class=\"link\" href=\"" + path + *it + "\">" + *it + "</a></li></center>");
	html.append("</ul>");
	html.append("<!-- made with <3 by catzGang --!>");
	return (std::vector<char> (html.begin(), html.end()));
}

std::pair<std::string, bool>	getIndex(std::vector<std::string> listOfFiles, std::vector<std::string>	listOfIndex)
{
	for (std::vector<std::string>::iterator it = listOfIndex.begin(); it != listOfIndex.end(); it++)
	{
		std::vector<std::string>::iterator findRes = std::find(listOfFiles.begin(), listOfFiles.end(), *it);
		if (findRes != listOfFiles.end())
			return (std::make_pair(*findRes, true));
	}
	return (std::make_pair("", false));
}

std::vector<std::string>	listingFile(std::string realPath)
{
	DIR							*d;
	struct dirent				*dir;
	std::vector<std::string>	listOfFiles;
	
	d = opendir(realPath.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
			listOfFiles.push_back(dir->d_name);
		closedir(d);
	}
	return (listOfFiles);
}
