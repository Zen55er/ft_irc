/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaiao-r <abaiao-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:58:00 by abaiao-r          #+#    #+#             */
/*   Updated: 2023/11/02 12:49:06 by abaiao-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

#include "Ft_irc.hpp"
#include "User.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "colours.hpp"

class Server
{
    private:
        int _port;
        int _server_fd;
        sockaddr_in _address;

        // private because we don't want to allow copies of this class
        Server(void);
        Server(const Server &src);
        Server &operator=(const Server &src);


    public:
        Server(int port);
        ~Server();

        int get_port(void) const;
        int get_server_fd(void) const;
        sockaddr_in get_address(void) const;


        int init_server(void);
        void listen_server(void);
        void close_server(void);
        
        // auxiliar functions of init_server
        int create_socket(void);
        int set_socket_options(void);
        int bind_socket(void);
        int start_listening(void);
    

};

#endif
