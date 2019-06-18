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
		return 0;

	//Creating object
	rcon_server Server1;

	//Connecting
	Server1.Bind("localhost", 27015);

	//Sending
	Server1.sendCommand(argv[1], argv[2]);

	//Printing
	Server1.printResault();

	//Closing Socket
	Server1.closeConnection();

	return 1;
}