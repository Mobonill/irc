/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:15:02 by morgane           #+#    #+#             */
/*   Updated: 2025/06/18 16:51:29 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "Utils.hpp"
#include <sstream>
#include <cstring>

std::vector<std::string> splitString(const std::string &str, const char *delim) {
    std::vector<std::string> tokens;
    char *token;
    char *cstr = strdup(str.c_str());
    char *tofree = cstr;

    while ((token = strtok(cstr, delim)) != NULL) {
        tokens.push_back(std::string(token));
        cstr = NULL;
    }

    free(tofree);
    return tokens;
}

bool isValid(char c, const std::string &validChars) {
    return validChars.find(c) != std::string::npos;
}

bool onlyValid(const std::string &str, const std::string &validChars) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (!isValid(str[i], validChars))
            return false;
    }
    return true;
}
