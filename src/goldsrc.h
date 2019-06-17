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
		char *nextToken, commandLine[256] = "\xFF\xFF\xFF\xFF challenge rcon\n";
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
		bool returnValue;
		FD_ZERO(&fds);
		FD_SET(Sock, &fds);
		select(Sock + 1, &fds, NULL, NULL, &timeout);
		try
		{
			if (FD_ISSET(Sock, &fds))
			{
				recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);
				if (recievedInfo[5] != NULL)
				{
					std::cout << std::endl << recievedInfo + 5 << std::endl;
					returnValue = true;
				}
				else
				{
					throw(101);
				}
			}
			else
				throw(102);
		}
		catch (int thrownInfo)
		{
			switch (thrownInfo)
			{
			case 101:
				std::cout << "\nNothing to Print\n\n";
				returnValue = false;
				break;
			case 102:
				std::cout << "\nNothing is set to the Socket, the Server is probably restarting!\n\n";
				returnValue = false;
				break;
			}
		}

		return returnValue;
	}

	void closeConnection()
	{
		closesocket(Sock);
	}
};