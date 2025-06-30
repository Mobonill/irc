/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:15:02 by morgane           #+#    #+#             */
/*   Updated: 2025/06/25 19:54:39 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string> splitString(const std::string& str, const char* delimiter)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + std::strlen(delimiter);
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start));

	return result;
}

bool isValid(const char &c, const std::string &valid)
{
	if (!c && valid.empty())
		return true;
	else if (valid.empty() || !c)
		return false;
	for (std::string ::const_iterator it = valid.begin(); it < valid.end(); it++)
	{
		if (c == *it)
			return true;
	}
	return false;
}

bool onlyValid(const std::string &s, const std::string &valid)
{
	if (s.empty() && valid.empty())
		return true;
	else if (s.empty() || valid.empty())
		return false;
	for (std::string ::const_iterator it = s.begin(); it < s.end(); it++)
	{
		if (!isValid(*it, valid))
			return false;
	}
	return true;
}
