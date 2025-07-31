/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:15:02 by morgane           #+#    #+#             */
/*   Updated: 2025/07/31 21:01:40 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "Utils.hpp"
#include <sstream>
#include <cstring>


const std::string	isValidPassword(const std::string &password)
{
	if (password.size() > 128)
		return "More than 128 char password.";
	bool onlyspace = true;
	for (size_t i = 0; i < password.size(); ++i)
	{
		if (!std::isprint(password[i]))
			return "Use of non-printable ascii char.";
		if (onlyspace == true && password[i] != ' ')
			onlyspace = false;
	}
	if (onlyspace)
		return "Only-space password detected.";
	else
		return "";
}

// std::vector<std::string> splitString(const std::string &str, const char *delim) {
//     std::vector<std::string> tokens;
//     char *token;
//     char *cstr = strdup(str.c_str());
//     char *tofree = cstr;

//     while ((token = strtok(cstr, delim)) != NULL) {
//         tokens.push_back(std::string(token));
//         cstr = NULL;
//     }

//     free(tofree);
//     return tokens;
// }

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

// bool isValid(char c, const std::string &validChars) {
//     return validChars.find(c) != std::string::npos;
// }

// bool onlyValid(const std::string &str, const std::string &validChars) {
//     for (size_t i = 0; i < str.size(); ++i) {
//         if (!isValid(str[i], validChars))
//             return false;
//     }
//     return true;
// }

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
