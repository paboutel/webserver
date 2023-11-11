/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 13:15:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/07 21:54:23 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>

std::vector<std::string> split_charset(std::string toSplit, std::string charset)
{
    std::vector<std::string> res;

    for (std::string::iterator it = toSplit.begin(); it < toSplit.end(); it++)
    {
        std::string buffer;
        while (it != toSplit.end() && charset.find(*it) == std::string::npos)
        {
            buffer.push_back(*it);
            it++;
        }
        if (!buffer.empty())
            res.push_back(buffer);
    }
    return (res);
}

std::vector<std::string> split(std::string s, std::string delimiter){
    std::vector<std::string> list;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
		if (!token.empty())
        	list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
	if (!s.empty())
    	list.push_back(s);
    return (list);
}

bool isStringEnquoted(const std::string str) { 
	int nQuote = 0;
	for (size_t i = 0; i < str.length(); i++) {
		if ('\"' == str.at(i)) { 
			nQuote ++; 
		} 
	} return nQuote == 2;
}

std::vector<std::string> split(std::string s, char delimiter, int start){
    std::vector<std::string> result;
	s.erase(0, start);
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return (result);
}

