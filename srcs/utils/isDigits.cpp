/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isDigits.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 19:13:13 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 19:16:31 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	isDigits(std::string &digits)
{
	for (std::string::iterator it = digits.begin(); it != digits.end(); it++)
	{
		if (*it < '0' || *it > '9')
			return (false);
	}
	return (true);
}
