#include <iostream>
#include "query.h"

int main(int argc, char** argv)
{
	//Argument Check
	if (argc < 2)
	{
		std::cout << "usage: <pass> <cmd>\n";
		return 0;
	}

	//Starting Up With Sockets
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		printf("Can't setup Sockets");
		return 0;
	}

	//Creating object
	BASIC_QUERY server("185.148.145.232", 27021);

	//Connecting
	if (!server.Connect())
	{
		printf("Can't connect to server");
	}

	//Sending players command;
	/*server.sendCommand(A2S_INFO);
	if (server.recieveInfo())
	{
		server.printResault(6);
	}*/

	//Server1.printResault();

	//Sending the actual rcon command
	server.sendRconCommand(argv[1], argv[2]);
	server.recieveInfo();
	server.printResault(5);

	return 1;
}