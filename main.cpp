/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:20:10 by morgane           #+#    #+#             */
/*   Updated: 2025/06/18 17:17:57 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Syntax error: ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    int port = std::atoi(argv[1]);
    std::string password(argv[2]);
    Server server(port, password);
    
    g_signal = &server;
    signal(SIGINT, Server::handleSignal);
    signal(SIGQUIT, Server::handleSignal);
    
    try {
        
        server.createServer();
        while(server.getSignal() == false)
            server.socketChecker();
        server.closeFds();
    } catch (const std::exception &e) {
        server.closeFds();
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }

    return (0);
}
