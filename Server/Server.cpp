#include "Server.h"

#include <winsock.h>
#include <WS2tcpip.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <ctime>

#include <Protocol/Protocol.h>


struct Player
{
	unsigned short port; // their id;
	struct sockaddr_in si_other;
	PlayerState* state;
	UserInputState input;
};

struct Bullet {
	//BulletState* state;
	double timeSinceShot;
};


std::vector<Player> mPlayers;
std::vector<Bullet> mBullets;
unsigned int numPlayersConnected = 0;
double totalBulletTime = 2.0;
GameSceneState gs;


void ShootBullet(BulletState& b, PlayerState* p, glm::vec4 dir)
{
	b.state = BulletState::SHOT;
	b.posX = p->posX;
	b.posZ = p->posZ;
	b.velX = dir.x * 1.0f;
	b.velZ = dir.z * 1.0f;
}

PlayerState* AddPlayerToGame()
{
	PlayerState newPlayer;
	newPlayer.state = PlayerState::ALIVE;
	newPlayer.posX = 0.0f;
	newPlayer.posZ = 0.0f;
	newPlayer.velX = 0.0f;
	newPlayer.velZ = 0.0f;
	newPlayer.rot = 0.0f;

	gs.players.push_back(newPlayer);

	return &(gs.players[gs.players.size() - 1]);
}

Player* GetPlayerByPort(unsigned short port, struct sockaddr_in si_other)
{
	// If a player with this port is already connected, return it
	for (int i = 0; i < mPlayers.size(); i++)
	{
		if (mPlayers[i].port == port)
		{
			printf("port:%d x,z:(%f,%f) rot:(%f) Get Player\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
			return &(mPlayers[i]);
		}
	}

	// Otherwise create a new player, and return that one!
	mPlayers[numPlayersConnected].port = port;
	mPlayers[numPlayersConnected].state = AddPlayerToGame();
	mPlayers[numPlayersConnected].input.id = port;
	mPlayers[numPlayersConnected].si_other = si_other;
	return &(mPlayers[numPlayersConnected++]);
}


const float UPDATES_PER_SEC = 60;
const float SENDS_PER_SEC = 5;
std::clock_t prevUpdate, prevSend, curr;
double elapsed_secs_since_update, elapsed_secs_since_send;


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

Server::Server(void)
	: mIsRunning(false)
	, mListenSocket(INVALID_SOCKET)
	, mAcceptSocket(INVALID_SOCKET)
{
	mPlayers.resize(4);
	mBullets.resize(4);

	// WinSock vars
	WSAData		WSAData;
	int			iResult;
	int			Port = 5150;
	SOCKADDR_IN ReceiverAddr;

	iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0)
	{
		PrintWSAError();
		return;
	}

	// Step #1 Create a socket
	mListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mListenSocket == INVALID_SOCKET)
	{
		PrintWSAError();
		return;
	}

	// Step #2 Bind our socket
	ReceiverAddr.sin_family = AF_INET;
	ReceiverAddr.sin_port = htons(Port);
	ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	iResult = bind(mListenSocket, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
	if (iResult == SOCKET_ERROR)
	{
		PrintWSAError();
		return;
	}

	// Set our socket to be nonblocking
	SetNonBlocking(mListenSocket);

	// Our server is ready 
	printf("[SERVER] Receiving IP: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
	printf("[SERVER] Receiving Port: %d\n", htons(ReceiverAddr.sin_port));
	printf("[SERVER] Ready to receive a datagram...\n");

	mIsRunning = true;
	prevUpdate = std::clock();
	prevSend = std::clock();
}

Server::~Server(void)
{
	closesocket(mListenSocket);
}

void Server::Update(void)
{
	if (!mIsRunning) return;

	// TODO: ReadData, SendData
	ReadData();
	curr = std::clock();
	elapsed_secs_since_update = (curr - prevUpdate) / double(CLOCKS_PER_SEC);

	if (elapsed_secs_since_update >= (1.0f / UPDATES_PER_SEC))
	{
		UpdatePlayers();
		UpdateBullets(elapsed_secs_since_update);
		prevUpdate = curr;
	}

	elapsed_secs_since_send = (curr - prevSend) / double(CLOCKS_PER_SEC);
	if (elapsed_secs_since_send >= (1.0f / SENDS_PER_SEC))
	{
		BroadcastUpdate();
		prevSend = curr;
	}

}


void Server::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR)
	{
		PrintWSAError();
		return;
	}
}

void Server::ReadData(void)
{
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	char buffer[512];

	int result = recvfrom(mListenSocket, buffer, 512, 0, (struct sockaddr*) & si_other, &slen);
	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// printf(".");		// Quick test
			return;
		}
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our buffer, and just
		// ignore this.
		memset(buffer, '\0', 512);
		return;
	}

	unsigned short port = si_other.sin_port;

	Player* player = GetPlayerByPort(port, si_other);
	player->input.Deserialize(buffer);

	printf("%d : %hu received { %d }\n", (int)mListenSocket, port, (int)player->input.input);
}

void Server::UpdatePlayers(void)
{
	for (unsigned int i = 0; i < numPlayersConnected; i++)
	{
		//printf("Index:%d player:%d input:%d state:%d\n", i, mPlayers[i].port, mPlayers[i].input.input, mPlayers[i].state->state);
		if (mPlayers[i].input.input == UserInputState::FORWARD)
		{
			glm::vec4 dir = (glm::mat4(glm::quat(glm::radians(glm::vec3(0, mPlayers[i].state->rot, 0)))) * glm::vec4(0, 0, 1, 1.0f));
			mPlayers[i].state->velX = dir.x * 5.0f;
			mPlayers[i].state->velZ = dir.z * 5.0f;
			dir *= (5.0f * elapsed_secs_since_update);
			mPlayers[i].state->posX += dir.x;
			mPlayers[i].state->posZ += dir.z;
			printf("port:%d x,z:(%f,%f) rot:(%f) FORWARDS!\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
		}
		else if (mPlayers[i].input.input == UserInputState::BACKWARD)
		{
			glm::vec4 dir = (glm::mat4(glm::quat(glm::radians(glm::vec3(0, mPlayers[i].state->rot, 0)))) * glm::vec4(0, 0, 1, 1.0f));
			mPlayers[i].state->velX = dir.x * -5.0f;
			mPlayers[i].state->velZ = dir.z * -5.0f;
			dir *= (5.0f * elapsed_secs_since_update);
			mPlayers[i].state->posX -= dir.x;
			mPlayers[i].state->posZ -= dir.z;
			printf("port:%d x,z:(%f,%f) rot:(%f) BACKWARDS!\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
		}
		else
		{
			mPlayers[i].state->velX = 0.0f;
			mPlayers[i].state->velZ = 0.0f;
		}

		if (mPlayers[i].input.input == UserInputState::TURN_LEFT)
		{
			mPlayers[i].state->rot += (5.0f * elapsed_secs_since_update);
			printf("port:%d x,z:(%f,%f) rot:(%f) LEFT!\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
		}
		else if (mPlayers[i].input.input == UserInputState::TURN_RIGHT)
		{
			mPlayers[i].state->rot -= (5.0f * elapsed_secs_since_update);
			printf("port:%d x,z:(%f,%f) rot:(%f) RIGHT!\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
		}
		else if (mPlayers[i].input.input == UserInputState::FIRE)
		{
			printf("port:%d x,z:(%f,%f) rot:(%f) SHOOOOOT!!!!\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);

			glm::vec4 dir = (glm::mat4(glm::quat(glm::radians(glm::vec3(0, mPlayers[i].state->rot, 0)))) * glm::vec4(0, 0, 1, 1.0f));

			// Check that bullet is loaded and not shot
			if (gs.bullets[i].state == BulletState::LOADED) 
			{
				ShootBullet(gs.bullets[i], mPlayers[i].state, dir);
				//mBullets[i].state = &gs.bullets[i];
				mBullets[i].timeSinceShot = 0.0f;
			}		

		}
	}
}

void Server::UpdateBullets(double timePassed) 
{
	for (unsigned int i = 0; i < numPlayersConnected; i++) 
	{
		// Check if time limit for bullet has expired
		if (mBullets[i].timeSinceShot >= totalBulletTime) {
			gs.bullets[i].state = BulletState::LOADED;
			mBullets[i].timeSinceShot = 0;
		}

		// Update the position of a shot bullet
		if (gs.bullets[i].state == BulletState::SHOT)
		{
			gs.bullets[i].posX += gs.bullets[i].velX;
			gs.bullets[i].posZ += gs.bullets[i].velZ;
		}

		// Time moves ever onwards...
		mBullets[i].timeSinceShot += timePassed;
	}
}

void Server::BroadcastUpdate(void)
{
	// create our data to send, then send the same data to all players
	const int DEFAULT_BUFLEN = 512;
	//char* buffer = '\0';
	//memset(buffer, '\0', DEFAULT_BUFLEN);

	//memcpy(&(buffer[0]), &numPlayersConnected, sizeof(unsigned int));



	//for (unsigned int i = 0; i < numPlayersConnected; i++)
	//{
	//	gs.players.push_back(mPlayers[i].state);
	//	printf("%f\n", mPlayers[i].state.posZ);
	//}
	std::vector<uint8_t> buff;
	gs.Serialize(buff);



	for (unsigned int i = 0; i < numPlayersConnected; i++)
	{
		buff[0] = i;
		buff[1] = i >> 8;
		buff[2] = i >> 16;
		buff[3] = i >> 24;
		printf("port:%d x,z:(%f,%f) rot:(%f) Broadcast\n", mPlayers[i].port, mPlayers[i].state->posX, mPlayers[i].state->posZ, mPlayers[i].state->rot);
		int result = sendto(mListenSocket, (char*)&buff[0], buff.size(), 0, (struct sockaddr*) & (mPlayers[i].si_other), sizeof(mPlayers[i].si_other));
	}


}
