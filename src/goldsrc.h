#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class rcon_server
{
private:
	SOCKET Sock = socket(AF_INET, SOCK_DGRAM, NULL);
	SOCKADDR_IN sin, to_addr;
	char recievedInfo[1400];

public:
	rcon_server(const char ipAddress[], short port)
	{
		//Setting addr
		ZeroMemory(&sin, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		InetPton(AF_INET, ipAddress, &sin.sin_addr);
	}
	
	~rcon_server()
	{
		closesocket(Sock);
	}
	
	bool Connect(const char ipAddress[], short port)
	{
		if (connect(Sock, (const SOCKADDR*)&sin, sizeof(sin)) != 0)
		{
			return false;
		}

		return true;
	}

	void sendCommand(char password[], char command[256])
	{
		//Challenging Rcon
		char commandLine[256] = "\xFF\xFF\xFF\xFF challenge rcon\n";
		send(Sock, commandLine, strlen(commandLine) + 1, NULL);
		recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);

		//Removing \n
		for (int i = 0; i < strlen(recievedInfo); i++)
		{
			if (recievedInfo[i] == '\n')
			{
				recievedInfo[i] = NULL;
			}
		}

		//Sending Cmd
		sprintf_s(commandLine, "\xFF\xFF\xFF\xFF rcon %s \"%s\" %s", recievedInfo + 19, password, command);
		send(Sock, commandLine, strlen(commandLine) + 1, NULL);
	}

	bool printResault()
	{
		////Recieving Info
		recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);
		if (recievedInfo[5] != NULL)
		{
			std::cout << std::endl << recievedInfo + 5 << std::endl;
			return true;
		}

		return false;
	}
};
