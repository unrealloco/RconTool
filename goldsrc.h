#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class rcon_server
{
private:
	SOCKET Sock = socket(AF_INET, SOCK_DGRAM, NULL);
	SOCKADDR_IN sin;
	char recievedInfo[1400];

public:
	void Bind(const char ipAddress[], short port)
	{
		//Setting addr
		ZeroMemory(&sin, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		InetPton(AF_INET, ipAddress, &sin.sin_addr);
	}

	void sendCommand(char password[], char command[256])
	{
		//Challenging Rcon
		char* nextToken, commandLine[256] = "\xFF\xFF\xFF\xFF challenge rcon\n";
		sendto(Sock, commandLine, sizeof(commandLine), NULL, (SOCKADDR*)(&sin), sizeof(sin));
		recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);
		strtok_s(recievedInfo, "\n", &nextToken);

		//Sending Cmd
		sprintf_s(commandLine, "\xFF\xFF\xFF\xFF rcon %s \"%s\" %s", recievedInfo + 19, password, command);
		sendto(Sock, commandLine, sizeof(commandLine), NULL, (SOCKADDR*)(&sin), sizeof(sin));
	}

	bool printResault()
	{
		////Recieving Info
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 200000;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(Sock, &fds);
		select(Sock + 1, &fds, NULL, NULL, &timeout);
		if (FD_ISSET(Sock, &fds))
		{
			recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);
			std::cout << std::endl << recievedInfo + 5 << std::endl;

			return true;
		}

		return false;
	}

	void closeConnection()
	{
		closesocket(Sock);
	}
};