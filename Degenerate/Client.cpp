#include "Client.h"


void _PrintWSAError(const char* file, int line)
{
	int WSAErrorCode = WSAGetLastError();
	wchar_t* s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				   NULL, WSAErrorCode,
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				   (LPWSTR)&s, 0, NULL);
	fprintf(stderr, "[WSAError:%d] %S\n", WSAErrorCode, s);
	LocalFree(s);
}


Client::Client(void)
	: mServerSocket(INVALID_SOCKET), si_other(), m_gs(new GameSceneState())
{
	WSAData		WSAData;
	int			iResult;
	int			Port = 5150;
	SOCKADDR_IN ReceiverAddr;

	// Step #0 Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0)
	{
		PrintWSAError();
		return;
	}
}

Client::~Client(void)
{
	closesocket(mServerSocket);
	WSACleanup();
}

void Client::CreateSocket(std::string ip, int port)
{
	mServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mServerSocket == SOCKET_ERROR)
	{
		PrintWSAError();
		return;
	}

	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	si_other.sin_addr.s_addr = inet_addr(ip.c_str());

	SetNonBlocking(mServerSocket);
}

bool Client::Update(void)
{
	return Recv();
}

void Client::Send(UserInputState i)
{
	std::vector<uint8_t> data;
	i.id = m_gs->id;
	i.Serialize(data);

	int result = sendto(mServerSocket, (char*)&data[0], data.size(), 0,
		(struct sockaddr*) & si_other, sizeof(si_other));

	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
		PrintWSAError();
		return;
	}

	if (result == 0)
	{
		printf("Disconnected...\n");
		return;
	}
	printf("Send: %s\n", std::to_string(i.input).c_str());
}

int Client::GetId()
{
	return m_gs->id;
}

void Client::getGameState(GameSceneState* &gs)
{
	gs = m_gs;
}

//void Client::SetPosition(int id, float& x, float& y)
//{
//}

void Client::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR)
	{
		PrintWSAError();
		return;
	}
}

bool Client::Recv(void)
{
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	char buffer[512];

	int result = recvfrom(mServerSocket, buffer, 512, 0, (struct sockaddr*) & si_other, &slen);
	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return false;
		}
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our buffer, and just
		// ignore this.
		memset(buffer, '\0', 512);
		return false;
	}

	//std::vector<uint8_t> buff(buffer);
	m_gs->Deserialize(buffer);

	//printf("%zd players, %zd\n", m_gs->players.size(), m_gs->bullets.size());
	return true;

}
