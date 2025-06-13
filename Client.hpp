/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/06/13 17:00:35 by mobonill         ###   ########.fr       */
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