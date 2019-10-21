#include "query.h"

// Public
BASIC_QUERY::BASIC_QUERY(const char ipAddress[], short port)
{
	//Setting addr
	ZeroMemory(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	InetPton(AF_INET, ipAddress, &sin.sin_addr);

	//Setting timeout
	ZeroMemory(&timeout, sizeof(timeout));
	timeout.tv_sec = 1;

	//Zeroing fds
	FD_ZERO(&fds);
}

BASIC_QUERY::~BASIC_QUERY()
{
	closesocket(Sock);
}

bool BASIC_QUERY::Connect()
{
	if (connect(Sock, (const SOCKADDR*)& sin, sizeof(sin)) != 0)
	{
		return false;
	}

	return true;
}

void BASIC_QUERY::formatCommand(char Type)
{
	if (Type == A2S_RULES || Type == A2S_PLAYER)
	{
		ZeroMemory(commandLine, sizeof(commandLine));
		if (challengeCode == 0)
		{
			*(long*)(commandLine + 0) = ~0;
			*(char*)(commandLine + 4) = Type;
			*(long*)(commandLine + 5) = ~0;
		}
		else
		{
			*(long*)commandLine = ~0;
			*(char*)(commandLine + 4) = Type;
			*(long*)(commandLine + 5) = challengeCode;
		}

		commandLenght = 9;
	}
	else if (Type == A2S_INFO)
	{
		sprintf_s(commandLine, "\xFF\xFF\xFF\xFF%cSource Engine Query.\x00", A2S_INFO);
		commandLenght = 25;
	}
}

bool BASIC_QUERY::sendCommand(char Type)
{
	challengeCode = 0;
	formatCommand(Type);

	if (Type == A2S_RULES || Type == A2S_PLAYER)
	{
		printf("First Command: \n%s\n", commandLine);
		send(Sock, commandLine, commandLenght, NULL);
		clearRecv();
		recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);

		if (recievedInfo[4] != S2C_CHALLENGE)
		{
			printf("Wrong challenge recieved");
			return false;
		} 

		printf("Second Command: \n%s\n", recievedInfo + 5);
		challengeCode = *(long*)(recievedInfo + 5);
		formatCommand(Type);
		printf("Third Command:\n%s\n", commandLine);
	}
	send(Sock, commandLine, commandLenght, NULL);

	return true;
}

bool BASIC_QUERY::sendRconCommand(char password[], char command[256])
{
	//Challenging Rcon
	char commandLine[256] = "\xFF\xFF\xFF\xFF challenge rcon\x00";
	send(Sock, commandLine, (int)strlen(commandLine) + 1, NULL);

	if (!checkSet())
	{
		return false;
	}

	recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);

	//Cutting the info
	for (int i = 0; i < (int)strlen(recievedInfo); i++)
	{
		if (recievedInfo[i] == '\n')
		{
			recievedInfo[i] = NULL;
		}
	}

	//Sending Cmd
	sprintf_s(commandLine, "\xFF\xFF\xFF\xFF rcon %s \"%s\" %s", recievedInfo + 19, password, command);
	send(Sock, commandLine, (int)strlen(commandLine) + 1, NULL);

	return true;
}

bool BASIC_QUERY::recieveInfo()
{
	//Revieving Info
	clearRecv();

	if (checkSet())
	{
		recv(Sock, recievedInfo, sizeof(recievedInfo), NULL);
		return true;
	}

	printf("Failed to recieve info.\n");
	return false;
}

char* BASIC_QUERY::getRecievedInfo(char* destination)
{
	char* source = recievedInfo;
	char* start = destination;

	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';
	return start;
}

// Debug
void BASIC_QUERY::printResault(int startPoint)
{
	////Printing Info
	printf("Recieved Info:\n%s\n", recievedInfo + startPoint);
}

// Protected
inline void BASIC_QUERY::clearRecv()
{
	ZeroMemory(recievedInfo, sizeof(recievedInfo));
}

inline bool BASIC_QUERY::checkSet()
{
	FD_SET(Sock, &fds);
	select((int)Sock + 1, &fds, NULL, NULL, &timeout);

	if (FD_ISSET(Sock, &fds))
	{
		return true;
	}

	return false;
}