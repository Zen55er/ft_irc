## What is it?
An IRC server designed to work with Hexchat or the netcat command.

The server:
- Handles multiple clients
- Uses epoll to avoid blocking connections
- Allows users to login, join channels and message other users
- Allows channel owners to customize and moderate the channel
- Allows for file transfers between users
- Has a simple bot that shows users how to use the channel and auto moderates messages inside channels

## How to use this?
1 - Clone this repository

2 - Run make

3 - Run `./ircserv <port> <password>`

4 - Install Hexchat and add the server created in the previous step

5 - Connect to the server with your choice of credentials

6 - Use the friendly bot, the command to see all other commands is "HALP"

## Group
[André Cândido da Silva](https://github.com/abaiao-r)\
[Gabriel Franco](https://github.com/Zen55er)\
[João Pereira](https://github.com/joao-per)
