/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/06/11 23:22:19 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once


class Client {
    
    private:
        int _clientSocket;

    public:
        Client(int clientSock);
        ~Client();
        
};