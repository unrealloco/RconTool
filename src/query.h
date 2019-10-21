#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define A2S_INFO '\x54'
#define A2S_PLAYER '\x55'
#define A2S_RULES '\x56'
#define A2S_SERVERQUERY_GETCHALLENGE '\x57' // deprecated
#define A2S_PING '\x69' // deprecated

#define S2C_CHALLENGE '\x41'

class BASIC_QUERY
{
private:
	SOCKET Sock = socket(AF_INET, SOCK_DGRAM, NULL);
	SOCKADDR_IN sin;
	FD_SET fds;
	TIMEVAL timeout;

	char recievedInfo[1400];
	char commandLine[256];

	short commandLenght;
	long challengeCode = 0;
protected:
	inline void clearRecv();
	inline bool checkSet();
	void formatCommand(char Type);
public:
	// Constructor & Destructor
	BASIC_QUERY(const char ipAddress[], short port);
	~BASIC_QUERY();
	// Connecting
	bool Connect();
	// Sending command
	bool sendCommand(char Type);
	bool sendRconCommand(char password[], char command[256]);
	// Recieveing the info
	bool recieveInfo();
	// Formatting the destination with the recieved info
	char* getRecievedInfo(char* destination);
	// (DEBUG) Printing the Resault
	void printResault(int startPoint);
};