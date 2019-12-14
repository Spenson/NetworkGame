#include "Protocol.h"

void UserInputState::Serialize(std::vector<uint8_t>& buff)
{
	buff.resize(8);
	buff[0] = id;
	buff[1] = id >> 8;
	buff[2] = id >> 16;
	buff[3] = id >> 24;

	buff[4] = input;
	buff[5] = input >> 8;
	buff[6] = input >> 16;
	buff[7] = input >> 24;
}

void UserInputState::Deserialize(std::vector<uint8_t> buff)
{
	id = buff[0] | buff[1] << 8 | buff[2] << 16 | buff[3] << 24;
	input = (Input)(buff[4] | buff[5] << 8 | buff[6] << 16 | buff[7] << 24);
}

void UserInputState::Deserialize(char* buff)
{
	id = (uint8_t)buff[0] | (uint8_t)buff[1] << 8 | (uint8_t)buff[2] << 16 | (uint8_t)buff[3] << 24;
	input = (Input)((uint8_t)buff[4] | (uint8_t)buff[5] << 8 | (uint8_t)buff[6] << 16 | (uint8_t)buff[7] << 24);
}

void PlayerState::Serialize(std::vector<uint8_t>& buff)
{
	buff.resize(24);

	buff[0] = state;
	buff[1] = state >> 8;
	buff[2] = state >> 16;
	buff[3] = state >> 24;

	uint32_t temp = *reinterpret_cast<uint32_t*>(&posX); // want to use bitshift and didn't work with float
	buff[4] = temp;
	buff[5] = temp >> 8;
	buff[6] = temp >> 16;
	buff[7] = temp >> 24;

	temp = *reinterpret_cast<uint32_t*>(&posZ); // want to use bitshift and didn't work with float
	buff[8] = temp;
	buff[9] = temp >> 8;
	buff[10] = temp >> 16;
	buff[11] = temp >> 24;



	temp = *reinterpret_cast<uint32_t*>(&velX); // want to use bitshift and didn't work with float
	buff[12] = temp;
	buff[13] = temp >> 8;
	buff[14] = temp >> 16;
	buff[15] = temp >> 24;

	temp = *reinterpret_cast<uint32_t*>(&velZ); // want to use bitshift and didn't work with float
	buff[16] = temp;
	buff[17] = temp >> 8;
	buff[18] = temp >> 16;
	buff[19] = temp >> 24;



	temp = *reinterpret_cast<uint32_t*>(&rot); // want to use bitshift and didn't work with float
	buff[20] = temp;
	buff[21] = temp >> 8;
	buff[22] = temp >> 16;
	buff[23] = temp >> 24;

}

void PlayerState::Deserialize(std::vector<uint8_t> buff)
{
	state = (State)(buff[0] | buff[1] << 8 | buff[2] << 16 | buff[3] << 24);

	uint32_t temp = (buff[4] | buff[5] << 8 | buff[6] << 16 | buff[7] << 24);
	posX = *reinterpret_cast<float*>(&temp);
	temp = (buff[8] | buff[9] << 8 | buff[10] << 16 | buff[11] << 24);
	posZ = *reinterpret_cast<float*>(&temp);
	temp = (buff[12] | buff[13] << 8 | buff[14] << 16 | buff[15] << 24);
	velX = *reinterpret_cast<float*>(&temp);
	temp = (buff[16] | buff[17] << 8 | buff[18] << 16 | buff[19] << 24);
	velZ = *reinterpret_cast<float*>(&temp);
	temp = (buff[20] | buff[21] << 8 | buff[22] << 16 | buff[23] << 24);
	rot = *reinterpret_cast<float*>(&temp);

}

void PlayerState::Deserialize(char* buff)
{
	state = (State)((uint8_t)buff[0] | (uint8_t)buff[1] << 8 | (uint8_t)buff[2] << 16 | (uint8_t)buff[3] << 24);

	uint32_t temp = ((uint8_t)buff[4] | (uint8_t)buff[5] << 8 | (uint8_t)buff[6] << 16 | (uint8_t)buff[7] << 24);
	posX = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[8] | (uint8_t)buff[9] << 8 | (uint8_t)buff[10] << 16 | (uint8_t)buff[11] << 24);
	posZ = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[12] | (uint8_t)buff[13] << 8 | (uint8_t)buff[14] << 16 | (uint8_t)buff[15] << 24);
	velX = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[16] | (uint8_t)buff[17] << 8 | (uint8_t)buff[18] << 16 | (uint8_t)buff[19] << 24);
	velZ = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[20] | (uint8_t)buff[21] << 8 | (uint8_t)buff[22] << 16 | (uint8_t)buff[23] << 24);
	rot = *reinterpret_cast<float*>(&temp);
}

void BulletState::Serialize(std::vector<uint8_t>& buff)
{
	buff.resize(20);

	buff[0] = state;
	buff[1] = state >> 8;
	buff[2] = state >> 16;
	buff[3] = state >> 24;

	uint32_t temp = *reinterpret_cast<uint32_t*>(&posX); // want to use bitshift and didn't work with float
	buff[4] = temp;
	buff[5] = temp >> 8;
	buff[6] = temp >> 16;
	buff[7] = temp >> 24;

	temp = *reinterpret_cast<uint32_t*>(&posZ); // want to use bitshift and didn't work with float
	buff[8] = temp;
	buff[9] = temp >> 8;
	buff[10] = temp >> 16;
	buff[11] = temp >> 24;

	temp = *reinterpret_cast<uint32_t*>(&velX); // want to use bitshift and didn't work with float
	buff[12] = temp;
	buff[13] = temp >> 8;
	buff[14] = temp >> 16;
	buff[15] = temp >> 24;

	temp = *reinterpret_cast<uint32_t*>(&velZ); // want to use bitshift and didn't work with float
	buff[16] = temp;
	buff[17] = temp >> 8;
	buff[18] = temp >> 16;
	buff[19] = temp >> 24;


}

void BulletState::Deserialize(std::vector<uint8_t> buff)
{
	state = buff[0] | buff[1] << 8 | buff[2] << 16 | buff[3] << 24;

	uint32_t temp = (buff[4] | buff[5] << 8 | buff[6] << 16 | buff[7] << 24);
	posX = *reinterpret_cast<float*>(&temp);
	temp = (buff[8] | buff[9] << 8 | buff[10] << 16 | buff[11] << 24);
	posZ = *reinterpret_cast<float*>(&temp);
	temp = (buff[12] | buff[13] << 8 | buff[14] << 16 | buff[15] << 24);
	velX = *reinterpret_cast<float*>(&temp);
	temp = (buff[16] | buff[17] << 8 | buff[18] << 16 | buff[19] << 24);
	velZ = *reinterpret_cast<float*>(&temp);
}

void BulletState::Deserialize(char* buff)
{
	state = (uint8_t)buff[0] | (uint8_t)buff[1] << 8 | (uint8_t)buff[2] << 16 | (uint8_t)buff[3] << 24;

	uint32_t temp = ((uint8_t)buff[4] | (uint8_t)buff[5] << 8 | (uint8_t)buff[6] << 16 | (uint8_t)buff[7] << 24);
	posX = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[8] | (uint8_t)buff[9] << 8 | (uint8_t)buff[10] << 16 | (uint8_t)buff[11] << 24);
	posZ = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[12] | (uint8_t)buff[13] << 8 | (uint8_t)buff[14] << 16 | (uint8_t)buff[15] << 24);
	velX = *reinterpret_cast<float*>(&temp);
	temp = ((uint8_t)buff[16] | (uint8_t)buff[17] << 8 | (uint8_t)buff[18] << 16 | (uint8_t)buff[19] << 24);
	velZ = *reinterpret_cast<float*>(&temp);
}

GameSceneState::GameSceneState() : id(-1), players(0), bullets(0)
{
	players.reserve(4);
	bullets.reserve(4);

	// 4 bullets for 4 players
	for (size_t i = 0; i < 4; i++)
	{
		BulletState newBullet;
		// 0 == Loaded
		// 1 == Shot
		newBullet.state = 0;
		newBullet.posX = 0.0f;
		newBullet.posZ = 0.0f;
		newBullet.velX = 0.0f;
		newBullet.velZ = 0.0f;
		bullets.push_back(newBullet);
	}
	
}


void GameSceneState::Serialize(std::vector<uint8_t>& buff)
{
	//size_t buffSize = 4 + (24 * players.size()) + (20 * bullets.size());
	buff.resize(12);

	buff[0] = id;
	buff[1] = id >> 8;
	buff[2] = id >> 16;
	buff[3] = id >> 24;

	// write players size
	buff[4] = (char)(players.size());
	buff[5] = (char)(players.size() >> 8);
	buff[6] = (char)(players.size() >> 16);
	buff[7] = (char)(players.size() >> 24);

	// write bullets size;
	buff[8] = (char)bullets.size();
	buff[9] = (char)bullets.size() >> 8;
	buff[10] = (char)bullets.size() >> 16;
	buff[11] = (char)bullets.size() >> 24;

	std::vector<uint8_t> temp;
	for (PlayerState p : players)
	{
		p.Serialize(temp);
		buff.insert(buff.end(), temp.begin(), temp.end());
	}

	for (BulletState b : bullets)
	{
		b.Serialize(temp);
		buff.insert(buff.end(), temp.begin(), temp.end());
	}

}

void GameSceneState::Deserialize(std::vector<uint8_t> buff)
{
	id = buff[0] | buff[1] << 8 | buff[2] << 16 | buff[3] << 24;
	size_t playersSize = buff[4] | buff[5] << 8 | buff[6] << 16 | buff[7] << 24;
	size_t bulletsSize = buff[8] | buff[9] << 8 | buff[10] << 16 | buff[11] << 24;

	size_t index = 12;
	players.clear();
	for (size_t playerNum = 0; playerNum < playersSize; playerNum++)
	{
		PlayerState p;
		p.Deserialize(std::vector<uint8_t>(buff.begin() + index, buff.begin() + (index + 24)));
		players.push_back(p);
		index += 24;
	}
	bullets.clear();
	for (size_t bulletNum = 0; bulletNum < bulletsSize; bulletNum++)
	{
		BulletState b;
		b.Deserialize(std::vector<uint8_t>(buff.begin() + index, buff.begin() + (index + 20)));
		bullets.push_back(b);
		index += 20;
	}
}

void GameSceneState::Deserialize(char* buff)
{
	id = (uint8_t)buff[0] | (uint8_t)buff[1] << 8 | (uint8_t)buff[2] << 16 | (uint8_t)buff[3] << 24;
	size_t playersSize = (uint8_t)buff[4] | (uint8_t)buff[5] << 8 | (uint8_t)buff[6] << 16 | (uint8_t)buff[7] << 24;
	size_t bulletsSize = (uint8_t)buff[8] | (uint8_t)buff[9] << 8 | (uint8_t)buff[10] << 16 | (uint8_t)buff[11] << 24;

	size_t index = 12;
	players.clear();
	for (size_t playerNum = 0; playerNum < playersSize; playerNum++)
	{
		PlayerState p;
		p.Deserialize(&buff[index]);
		players.push_back(p);
		index += 24;
	}
	bullets.clear();
	for (size_t bulletNum = 0; bulletNum < bulletsSize; bulletNum++)
	{
		BulletState b;
		b.Deserialize(&buff[index]);
		bullets.push_back(b);
		index += 20;
	}
}
