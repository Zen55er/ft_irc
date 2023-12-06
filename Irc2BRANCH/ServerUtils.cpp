/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaiao-r <abaiao-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 08:29:56 by gacorrei          #+#    #+#             */
/*   Updated: 2023/11/30 20:41:18 by abaiao-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Server: constructor
** 1. set loop state to 1
** 2. set server fd to -1
** 3. set port to 0
** 4. set password to empty string
** 5. set max clients to 0
** 6. set max fd to 0
** 7. set fd set to empty
** 8. set clients vector to empty
** 9. set channels vector to empty
*/
bool	Server::pass_validation(std::string check) const
{
	return (check == _password);
}

/* name_validation: check if name is valid
** 1. check if name is between 1 and 9 characters long
** 2. check if name starts with '#' or ':'
** 3. check if name has whitespaces
** 4. check if name has invalid characters
** 5. return true if name is valid
** 6. return false if name is invalid
*/
bool	Server::name_validation(std::string check)
{
	int	len = check.size();

	if (len > MAX_LEN || len < MIN_LEN || check[0] == '#' || check[0] == ':')
		return false;
	for (int i = 0; i < len; i++)
		if (std::isspace(check[i]) || (!std::isalnum(check[i])
			&& (check[i] != '_') && (check[i] != '-') && (check[i] != '[')
			&& (check[i] != ']') && (check[i] != '{') && (check[i] != '}')
			&& (check[i] != '\\') && (check[i] != '|')))
			return false;
	return true;
}

/* nick_validation: check if nickname is valid
** 1. check if nickname is valid
** 2. check if nickname is already in use
** 3. return 0 if nickname is valid and not in use
** 4. return 1 if nickname is invalid
** 5. return 2 if nickname is already in use
*/
int	Server::nick_validation(std::string check)
{
	if (!name_validation(check))
		return 1;

	C_IT	it = _clients.begin();

	for (; it != _clients.end(); it++)
	{
		if (name_compare(check, it->get_nickname()))
			return 2;
	}
	return 0;
}

/* name_compare: compare two strings
** 1. transform both strings to lowercase
** 2. compare strings
** 3. return 1 if strings are equal, 0 if not
*/
int	Server::name_compare(std::string check, std::string comp)
{
	std::cout << "Name to check is: " << check << " comparing against: " << comp << "\n";
	std::transform(check.begin(), check.end(), check.begin(), tolower);
	std::transform(comp.begin(), comp.end(), comp.begin(), tolower);
	std::cout << "After tolower: " << check << " and " << comp << "\n";
	if (check == comp)
		return 1;
	return 0;
}

/* disconnect_client: remove client from server
** 1. find client in _clients vector
** 2. remove client from all channels
** 3. close client's fd
** 4. remove client from _clients vector
*/
void	Server::disconnect_client(int fd)
{
	C_IT	end = _clients.end();
	C_IT	match = std::find(_clients.begin(), end, fd);

	if (match == end)
		throw(std::runtime_error("Error. Could not find client"));
	leave_all_rooms(*match);
	close(fd);
	_clients.erase(match);
}


/* disconnect_client: remove client from server
** 1. find client in _clients vector
** 2. remove client from all channels
** 3. close client's fd
** 4. remove client from _clients vector
*/
void	Server::disconnect_client(Client &client)
{
	int		fd = client.get_client_fd();
	C_IT	end = _clients.end();
	C_IT	match = std::find(_clients.begin(), end, fd);

	if (match == end)
		throw(std::runtime_error("Error. Could not find client"));
	leave_all_rooms(client);
	close(fd);
	_clients.erase(match);
}

/* leave_all_rooms: remove client from all channels
** 1. iterate through all channels
** 2. remove client from channel
** 3. remove client from channel's clients operator vector
** 4. check if channel is empty
** 5. if channel is empty, remove channel
*/
void	Server::leave_all_rooms(Client &client)
{
	CH_IT	it = _channels.begin();
	Client	remove = *find(_clients.begin(), _clients.end(), client.get_client_fd()).base();

	for (; it < _channels.end(); it++)
	{
		it->remove_client(remove);
		it->remove_client_from_clients_operator_vector(remove);
		it->check_operator();
		sendChannelUserListMessage(it.base(), client.get_nickname());
		if (it->get_clients_in_channel().size() == 0)
			_channels.erase(it);
	}
}

/* signal_handler: handle SIGINT signal
** 1. set loop state to 0
*/
void	Server::signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << std::endl;
		_loop_state = 0;
	}
}

/* channel_name_validation: check if channel name is valid
** 1. check if channel name starts with '#'
** 2. check if channel name has whitespaces
** 3. check if channel name is between 2 and 200 characters long
*/
int Server::channel_name_validation(int client_fd, std::string check)
{
	int	len = check.size();
	std::string message;

	if (check[0] != '#')
	{
		message = "Error[JOIN]: Channel name must start with '#'\r\n";
		sendMessage(client_fd, message);
		return (1);
	}
	for (int i = 1; i < len; i++)
	{
		if (!std::isalnum(check[i]) && (check[i] != '_') && (check[i] != '-'))
		{
			message = "Error[JOIN]: Channel name can only contain alphanumeric characters, '_' and '-'\r\n";
			sendMessage(client_fd, message);
			return (1);
		}
	}
	if (len < 2 || len > 200)
	{
		message = "Error[JOIN]: Channel name must be between 2 and 200 characters long\r\n";
		sendMessage(client_fd, message);
		return (1);
	}
	return (0);
}

/* is_client_admin: check if client is administrator
 * 1. Check if client is administrator
 * 2. Send error message if client is not administrator
 */
int Server::is_client_admin(Client &client) // Do we need this? Deprecated?
{
	if (!client.get_is_admin())
	{
		std::string error = "Error: " + client.get_nickname() 
			+ " is not administrator\r\n";
		sendMessage(client.get_client_fd(), error);
		return (-1);
	}
	return 0;
}

/* findChannel: find channel
 * 1. Find channel by iterating through _channels
 * 2. If channel does not exist, send error message to client and return NULL
 * else return channel
 */
Channel	*Server::findChannel(Client &client, const std::string	&channelName)
{
	std::vector<Channel>::iterator it = find(_channels.begin(), _channels.end(), channelName);

	(void)client;
	if (it == _channels.end())
	{
		return (NULL);
	}
	return &(*it);
}

/* find_client: find client
 * 1. Find client by iterating through _clients
 * 2. If client does not exist, send error message to client and return NULL
 * else return client
 */
Client	*Server::find_client(Client &client, const std::string& nickname)
{
	(void)client;

	std::vector<Client>::iterator it = find(_clients.begin(), _clients.end(), nickname);

	if (it == _clients.end())
		return (NULL);
	return &(*it);
}

/* password_checker: check if password is valid
 * 1. Check if password is between 3 and 12 characters
 * 2. Check if password contains non-printable characters
 */
int	Server::password_checker(std::string password)
{
	// Check if password is between 3 and 12 characters
	if (password.length() < 3 || password.length() > 12)
	{
		std::cerr << RED << "ERROR: " << RESET 
			<< "Password must be between 3 and 12 characters" << std::endl;
		return (1);
	}
	// Check if password contains non-printable characters
	for (size_t i = 0; i < password.length(); i++)
	{
		if (!isprint(password[i]))
		{
			std::cerr << RED << "ERROR: " << RESET 
				<< "Password must not contain non-printable characters" << std::endl;
			return (1);
		}
	}
	return (0);
}
/* password_checker: check if password is valid
 * 1. Check if password is between 3 and 12 characters
 * 2. Check if password contains only non-printable characters
 */
int	Server::password_checker(std::string password, int fd)
{
	// Check if password is between 3 and 12 characters
	if (password.length() < 3 || password.length() > 12)
	{
		std::string error = "Error. Password must be between 3 and 12 characters\r\n";
		sendMessage(fd, error);
		return (1);
	}
	// Check if password contains non-printable characters
	for (size_t i = 0; i < password.length(); i++)
	{
		if (!isprint(password[i]))
		{
			std::string error = "Error. Password must not contain non-printable characters\r\n";
			sendMessage(fd, error);
			return (1);
		}
	}
	return (0);
}

/* get_users_string: get users string
 * 1. Get list of clients in channel
 * 2. Iterate through list and add clients to string
 * 3. Return string
 */
std::string	Server::get_users_string(Channel &channel)
{
	std::string			ret;
	std::vector<Client>	list = channel.get_clients_in_channel();
	C_IT	it = list.begin();

	for (; it != list.end(); it++)
	{
		std::string	nickname = it->get_nickname();
		if (!channel.find_client(nickname, "operators"))
			ret += "%" + it->get_nickname();
		else
			ret += "@" + it->get_nickname();
		if (it + 1 != list.end())
			ret += " ";
	}
	if (ret.empty())
		ret = " ";
	return ret;
}

/* join_messages: send messages to client and channel
 * 1. Send JOIN message to client
 * 2. Send RPL_TOPIC message to client
 * 3. Send RPL_NAMREPLY message to client
 * 4. Send RPL_ENDOFNAMES message to client
 * 5. Send JOIN message to channel
 */
void	Server::join_messages(Client &client, Channel &channel)
{
	std::string	message;
	std::string	name = channel.get_name();
	int			fd = client.get_client_fd();

	message = ":" + client.get_nickname() + "!" + client.get_username() + "@" 
		+ "localhost" + " JOIN " + name + "\r\n";
	sendMessage(fd, message);
	message = ":localhost " + RPL_TOPIC + " " + client.get_nickname() + " " 
		+ name + " " + channel.get_topic() + "\r\n";
	sendMessage(fd, message);
	message = ":localhost " + RPL_NAMREPLY + " " + client.get_nickname() + " = " 
		+ name + " :" + get_users_string(channel) + "\r\n";
	channel.info_message(message);
	message = ":localhost " + RPL_ENDOFNAMES + " " + client.get_nickname() + " " 
		+ name + " :End of NAMES list\r\n";
	channel.info_message(message);
}

/* sendChannelUserListMessage: send channel user list message
 * 1. Send message to client with list of users in channel
 * 2. Send message to client with end of names list
 */
void Server::sendChannelUserListMessage(Channel *channel, const std::string &argument)
{
    std::string userListMessage = ":localhost " + RPL_NAMREPLY + " " + argument
		+ " = " + channel->get_name() + " :" + get_users_string(*channel) 
		+ "\r\n";
    channel->info_message(userListMessage);

    std::string endOfNamesMessage = ":localhost " + RPL_ENDOFNAMES + " " 
		+ argument + " " + channel->get_name() + " :End of NAMES list\r\n";
    channel->info_message(endOfNamesMessage);
}