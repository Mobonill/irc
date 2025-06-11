/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:15:02 by morgane           #+#    #+#             */
/*   Updated: 2025/06/11 22:18:40 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::vector<std::string> splitString(const std::string& str, const char* delimiter) {
    
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