/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intToStr.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 13:11:38 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 13:13:22 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>

std::string to_string(int nb) {
    std::ostringstream    stream;

    stream << nb;
    return (stream.str());
}
