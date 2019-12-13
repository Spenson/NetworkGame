#pragma once

#include "iCommand.h"

class MoveCommand : public iCommand
{

	std::string m_Name;
	bool m_Done;
	glm::vec3 m_End;
	glm::vec3 m_Start;
	cGameObject* m_pGO;
	float m_MaxSpeed;
	float m_AccelDis;
	float m_DecelDis;

	bool m_HasUpdated;

public:
	MoveCommand(std::string name) : m_Name(name), m_Done(false), m_HasUpdated(false),
		m_MaxSpeed(0), m_AccelDis(0), m_DecelDis(0),
		m_End(glm::vec3(0.0f)), m_Start(glm::vec3(0.0f)),
		m_pGO(nullptr)
	{};
	// Inherited via iCommand
	virtual void SetName(std::string name) override;
	virtual bool IsDone() override;
	virtual bool Update(double deltaTime) override;
	virtual void SetGameObject(cGameObject* pGO) override;
	virtual void Init(std::vector<sPair> vecDetails) override;
	virtual void AddCommand(iCommand* command) override;
};