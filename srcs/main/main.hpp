/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:17:00 by nfaivre           #+#    #+#             */
/*   Updated: 2022/11/07 23:04:51 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

extern bool g_exit;

class InvalidHost: public std::exception {
private:
	std::string	_error;
public:
	InvalidHost(const std::string &error)
	: _error(error) {}
	virtual ~InvalidHost(void) throw() {}
	virtual const char *what() const throw()
	{ return (_error.c_str()); }
};
