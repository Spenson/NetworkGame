#pragma once

// Y never changes for pos & vel
// Rotation is around Y

#include <vector>


class UserInputState
{
public:
	int id;
	enum Input
	{
		FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT, FIRE
	};
	Input input;
	void Serialize(std::vector<uint8_t>& buff);
	void Deserialize(std::vector<uint8_t> buff);
	void Deserialize(char* buff);
};


class PlayerState
{
public:
	enum State
	{
		ALIVE, DEAD, DISCONNECT
	};

	State state;
	float posX, posZ;
	float velX, velZ;
	float rot;
	void Serialize(std::vector<uint8_t>& buff);
	void Deserialize(std::vector<uint8_t> buff);
	void Deserialize(char* buff);
};


class BulletState
{
public:
	int state;
	float posX, posZ;
	float velX, velZ;
	void Serialize(std::vector<uint8_t>& buff);
	void Deserialize(std::vector<uint8_t> buff);
	void Deserialize(char* buff);
};


class GameSceneState
{
public:
	int id;
	std::vector<PlayerState> players;
	std::vector<BulletState> bullets;

	GameSceneState();
	void UpdateId(int id);
	void Serialize(std::vector<uint8_t>& buff);
	void Deserialize(std::vector<uint8_t> buff);
	void Deserialize(char* buff);
};