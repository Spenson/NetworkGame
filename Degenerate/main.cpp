#include "globals.h"

#include <iostream>

#include "UserInputs/GFLW_callbacks.h"
#include "Camera/ThirdPersonCamera.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "SimpleShaderStuff/UniformManager.h"
#include "Physics/cPhysics.h"
#include "LowPassFilter/cLowPassFilter.h"
#include "Lighting/cLightHelper.h"
#include "SimpleShaderStuff/Rendering.h"

#include "LoadingStuff/LoadScene.h"

#include <glm/gtc/type_ptr.hpp>

#include <sstream>


#include "Client.h"


cBasicTextureManager* g_pTextureManager = NULL;

bool ISALIVE = true;

bool bLightDebugSheresOn = false;


extern cGameObject* g_pDebugSphere;
extern cGameObject* g_pDebugCube;


extern glm::vec2 textOffest;


Client g_client;
glm::vec3 Vec2ToVec3(glm::vec2 v)
{
	return glm::vec3(v.x, 0.0f, v.y);
}

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1280, 720, "Degenerate", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	void ProcessAsyncMouse(GLFWwindow * window);
	void ProcessAsyncKeys(GLFWwindow * window, double deltaTime);
	void ShipControls(GLFWwindow * window, double deltaTime);






	//TODO: Move if needed
	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();


	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

	cVAOManager* pTheVAOManager = new cVAOManager();

	UniformManager::Init();

	// Texture stuff
	::g_pTextureManager = new cBasicTextureManager();

	cLightHelper* pLightHelper = new cLightHelper();

	std::string lighterrors;
	::g_pLightManager = new LightManager();
	::g_pLightManager->InitilizeLightUinformLocations(UniformManager::shaderProgID, "theLights", 50, lighterrors);

	//TODO: Rewrite Loading Model stuff
	TestLoad(pTheVAOManager, pTheModelLoader, UniformManager::shaderProgID, pDebugRenderer, ::g_vec_pGameObjects, ::g_pLightManager, ::g_pTextureManager);





	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing


	cPhysics* pPhsyics = new cPhysics();
	pPhsyics->setGravity(glm::vec3(0.0f));

	cLowPassFilter avgDeltaTimeThingy;


	double lastTime = glfwGetTime();

	cGameObject* collisionShpere = new cGameObject();
	collisionShpere->debugColour = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);
	collisionShpere->doNotLight = true;
	collisionShpere->meshName = "sphere_lowres";
	collisionShpere->friendlyName = "collision";
	collisionShpere->scale = 0.5f;
	collisionShpere->useDiffuse = true;






	::g_pFreeCamera->SetPosition(glm::vec3(0, 200, -200));
	::g_pFreeCamera->Pitch(20 * -40);
	//::g_pFreeCamera->Yaw(20 * -140);
	//std::cout << ::g_pFreeCamera->Yaw() << std::endl;

	ThirdPersonCamera tpc;
	tpc.SetPlayerObject(pFindObjectByFriendlyName("Ship1"));
	tpc.SetPositionRelitiveToObject(glm::vec3(0.0f, 10.0f, -50.0f));
	tpc.SetTargetRelitiveToObject(glm::vec3(0.0f, 0.0f, 0.0f));

	//tpc.SetPositionRelitiveToObject(glm::vec3(0.0f, 50.0f, -0.1f));

	::g_client.CreateSocket("127.0.0.1", 5150);


	//unsigned maxFPS = 60;
	//unsigned maxServerConnections
	//double timeSinceLastInput = 0.0f;



	while (!glfwWindowShouldClose(window))
	{


		for (size_t index = 0; index != (::g_vec_pGameObjects.size() - 1); index++)
		{
			glm::vec3 ObjA = ::g_vec_pGameObjects[index]->positionXYZ;
			glm::vec3 ObjB = ::g_vec_pGameObjects[index + 1]->positionXYZ;

			//if (glm::distance(ObjA, ::g_pFlyCamera->eye) < glm::distance(ObjB, ::g_pFlyCamera->eye))
			if (ISALIVE)
			{
				if (glm::distance(ObjA, tpc.Position()) < glm::distance(ObjB, tpc.Position()))
					std::swap(::g_vec_pGameObjects[index], ::g_vec_pGameObjects[index + 1]);
			}
			else
			{
				if (glm::distance(ObjA, ::g_pFreeCamera->GetPosition()) < glm::distance(ObjB, ::g_pFreeCamera->GetPosition()))
					std::swap(::g_vec_pGameObjects[index], ::g_vec_pGameObjects[index + 1]);
			}
		}// for (unsigned int index




		//Frame Time
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;


		lastTime = currentTime;
		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME)
			deltaTime = SOME_HUGE_TIME;

		avgDeltaTimeThingy.addValue(deltaTime);


		//Frame Rate in title bar
		std::stringstream ssTitle;
		ssTitle << "StarDog | " << 1.0 / avgDeltaTimeThingy.getAverage();
		glfwSetWindowTitle(window, ssTitle.str().c_str());










		if (ISALIVE)
		{
			ShipControls(window, deltaTime);
		}
		else
		{
			ProcessAsyncKeys(window, deltaTime);
			ProcessAsyncMouse(window);
		}





		if (::g_client.Update())
		{
			GameSceneState* temp = nullptr;
			g_client.getGameState(temp);
			tpc.SetPlayerObject(pFindObjectByFriendlyName("Ship" + std::to_string(temp->id + 1)));
			for (int i = 0; i < temp->players.size(); i++)
			{
				if(i == temp->id)
					ISALIVE = temp->players[i].state == PlayerState::ALIVE;


				PlayerState p = temp->players[i];
				cGameObject* object = pFindObjectByFriendlyName("Ship" + std::to_string(i + 1));
				object->positionXYZ.x = p.posX;
				object->positionXYZ.z = p.posZ;
				object->velocity.x = p.velX;
				object->velocity.z = p.velZ;
				object->isVisible = true;
				object->setOrientation(glm::vec3(0.0f, p.rot, 0.0f));

				if (temp->players[i].state == PlayerState::DEAD)
				{
					object->diffuseColour.a = 0.2;
				}
				else
				{
					object->diffuseColour.a = 1.0;
				}
			}

			for (int i = 0; i < temp->bullets.size(); i++)
			{
				BulletState b = temp->bullets[i];
				cGameObject* object = pFindObjectByFriendlyName("Bullet" + std::to_string(i + 1));
				object->positionXYZ.x = b.posX;
				object->positionXYZ.z = b.posZ;
				object->velocity.x = b.velX;
				object->velocity.z = b.velZ;
				object->isVisible = true;
				object->scale = 0.5f;

				if (b.state == BulletState::LOADED)
				{
					object->isVisible = false;
				}

			}
		}







		glUseProgram(UniformManager::shaderProgID);

		float ratio;
		int width, height;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		p = glm::perspective(0.6f, ratio, 0.5f, 10000.0f);
		v = glm::mat4(1.0f);
		//v = glm::lookAt(::g_pFlyCamera->eye, ::g_pFlyCamera->getAtInWorldSpace(), ::g_pFlyCamera->getUpVector());
		if (ISALIVE)
			v = glm::lookAt(tpc.Position(), tpc.Target(), tpc.UpVector());
		else
			v = glm::lookAt(::g_pFreeCamera->GetPosition(), ::g_pFreeCamera->GetTarget(), ::g_pFreeCamera->GetUpVector());

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		g_pLightManager->PassLightsToShader();
		if (ISALIVE)
			glUniform4f(UniformManager::eyeLocation_UL, tpc.Position().x, tpc.Position().y, tpc.Position().z, 1.0f);
		else
			glUniform4f(UniformManager::eyeLocation_UL, ::g_pFreeCamera->GetPosition().x, ::g_pFreeCamera->GetPosition().y, ::g_pFreeCamera->GetPosition().z, 1.0f);


		glUniformMatrix4fv(UniformManager::matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(UniformManager::matProj_UL, 1, GL_FALSE, glm::value_ptr(p));





















		double averageDeltaTime = avgDeltaTimeThingy.getAverage();

		// TODO: Update to include Angular Velocity
		pPhsyics->IntegrationStep(::g_vec_pGameObjects, deltaTime);//(float)averageDeltaTime);


		cGameObject* pSkyBox = pFindObjectByFriendlyName("skybox");
		if (ISALIVE)
			pSkyBox->positionXYZ = tpc.Position();
		else
			pSkyBox->positionXYZ = ::g_pFreeCamera->GetPosition();// ::g_pFlyCamera->eye;

		DrawObject(glm::mat4(1.0f), pSkyBox, UniformManager::shaderProgID, pTheVAOManager);

		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
			if (pCurrentObject->friendlyName == "skybox")
				continue;

			glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix

			DrawObject(matModel, pCurrentObject, UniformManager::shaderProgID, pTheVAOManager);

		}//for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)

		


		pDebugRenderer->RenderDebugObjects(v, p, 0.01f);



		glfwSwapBuffers(window);
		glfwPollEvents();


	}
	glfwDestroyWindow(window);
	glfwTerminate();

	delete pTheModelLoader;


	exit(EXIT_SUCCESS);
}



// Here because didn't know where else to put it and needed for the draw call
glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject, glm::mat4 matWorld)
{
	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
						 glm::vec3(pCurrentObject->positionXYZ.x,
								   pCurrentObject->positionXYZ.y,
								   pCurrentObject->positionXYZ.z));
	matWorld = matWorld * matTrans;
	// ******* TRANSLATION TRANSFORM *********




	//// ******* ROTATION TRANSFORM *********
	glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
	matWorld = matWorld * matRotation;
	// ******* ROTATION TRANSFORM *********




	// ******* SCALE TRANSFORM *********
	// TODO: change to vec3 scale
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
								 glm::vec3(pCurrentObject->scale,
										   pCurrentObject->scale,
										   pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********


	return matWorld;
}