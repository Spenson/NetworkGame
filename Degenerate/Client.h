#pragma once

#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#include <string>

#include <Protocol/Protocol.h>


void _PrintWSAError(const char* file, int line);
#define PrintWSAError() _PrintWSAError(__FILE__, __LINE__)

class Client
{
public:
	Client(void);
	~Client(void);

	void CreateSocket(std::string ip, int port);
	bool Update(void);

	void Send(UserInputState i);
	int GetId();
	void getGameState(GameSceneState* &gs);

	//void SetPosition(int id, float& x, float& y);
private:
	void SetNonBlocking(SOCKET socket);
	bool Recv(void);

	unsigned int ClientNumber;

	GameSceneState* m_gs;

	SOCKET mServerSocket;
	struct sockaddr_in si_other;
};