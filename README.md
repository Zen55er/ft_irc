<h1 align="center">
	🌐 FT_IRC 🌐
</h1>

<p align="center">
	<b><i> Bridging the Bytes for Seamless Conversations 🌉</i></b>
</p>

<div align="center">
<img alt="FT_IRC" src="https://github.com/joao-per/joao-per/blob/main/Badges/Ft_irc.png" />
</div>

## 🧠 About the project
A basic implementation of an IRC server that handles authentication, user registration, bot, file transfer and several chat commands. 🖥️

## 🌟 Features
🔒 **Authentication**: Authenticate users with a password.

🖋️ **Registration**: Register users by setting their nickname (NICK) and real name (USER).

📜 **Command Handling**: Process various commands like JOIN, BAN, TOPIC and PRIVMSG.

🧑‍🤝‍🧑 **Multi-Client Handling**: Uses poll() system call to handle multiple clients simultaneously.

🤖 **Bot**: Detects if a user sends messages with bad words in it.

🗂️ **File Transfer**: It's possible to send files through clients.

## 🚀 Getting Started
### 🛠️ Prerequisites
A C++ compiler with support for C++11 or newer.
Basic knowledge of IRC protocols (optional).

### 📖 Compilation
You can compile the project using:

```bash
make
```

And then run the server:
```bash
./ircserv <port> <password>
```

For example:
```bash
./ircserv 6667 12345
```

## 🙏 Acknowledgments
A huge thank you to my colleagues gacorrei and abaiao-r for their invaluable contributions and insights. This project wouldn't be the same without their assistance. 🙌

## 📝 License
This project is licensed under the MIT License - see the LICENSE file for details.
