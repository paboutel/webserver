/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   utils.hpp                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: nfaivre <nfaivre@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2022/10/24 21:21:21 by nfaivre          #+#    #+#              */
/*   Updated: 2022/10/24 21:21:26 by nfaivre         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include <string>
# include <map>

std::vector<std::string>							split(std::string s, std::string delimiter);
int													parseStartLine(char **tab, std::map< std::string, std::vector<std::string> > &map);
std::map< std::string, std::vector<std::string> >	createHttpRequest(std::string req);
int													parseHeaders(char **tab, std::map< std::string, std::vector<std::string> > &map);
void												print_tab(std::vector<std::string> tab);
std::vector<std::string> 							split(std::string s, char delimiter, int start);
int													countLenServer(std::istream &file);
int													ft_parse_server(std::istream &file, std::map< int, std::map<std::string, std::string> > &configFile);
std::string											getDate(void);
std::string                                     	to_string(int nb);
void												printMap(std::map< std::string, std::vector<std::string> > map);
bool												isDigits(std::string &digits);
std::vector<std::string>							listingFile(std::string realPath);
std::vector<char>									createIndexDir(std::vector<std::string> listOfFiles, std::string path);
std::pair<std::string, bool>						getIndex(std::vector<std::string> listOfFiles, std::vector<std::string>	listOfIndex);
std::vector<std::string>							split_charset(std::string toSplit, std::string charset);
