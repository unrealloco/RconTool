#include <iostream>
#include "goldsrc.h"

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
	rcon_server Server1("93.123.18.78", 27016);

	//Connecting
	if (!Server1.Connect())
	{
		printf("asd");
		return 0;
	}

	//Sending
	Server1.sendCommand(argv[1], argv[2]);

	//Printing
	Server1.printResault();

	return 1;
}
