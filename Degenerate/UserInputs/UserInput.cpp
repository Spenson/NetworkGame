#include "../globals.h"			// for find m_pGO

#include "GFLW_callbacks.h"
#include <stdio.h>		// for fprintf()
#include <iostream>

#include "../LoadingStuff/FileReaders.h"
#include "../Client.h"

bool isOnlyShiftKeyDown(int mods);
bool isOnlyCtrlKeyDown(int mods);
bool isOnlyAltKeyDown(int mods);
bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);

bool g_MouseIsInsideWindow = false;
bool g_MouseLeftButtonIsDown = false;

glm::vec3 inputRotation = glm::vec3(0.0f);
float speed = 3.0f;


const float CAMERASPEED = 0.5f;
unsigned SelecetedLight = 0;
extern bool bLightDebugSheresOn;


double const UPDATES_PER_SEC = 5;
double const UPDATE_FREQ = 1.0 / UPDATES_PER_SEC;

double cur = 0.0;

extern Client g_client;
// Declared in theMain
//extern cFlyCamera* g_pFlyCamera;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
	}
	return;
}//cursor_enter_callback(...

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}




bool isOnlyShiftKeyDown(int mods)
{
	if (mods == GLFW_MOD_SHIFT)
	{
		// Shift key is down all by itself
		return true;
	}
	return false;
}

bool isOnlyCtrlKeyDown(int mods)
{
	if (mods == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;
}

bool isOnlyAltKeyDown(int mods)
{
	if (mods == GLFW_MOD_ALT)
	{
		return true;
	}
	return false;
}

bool isShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	//::g_pFlyCamera->setMouseWheelDelta(yoffset);
	::g_pFreeCamera->Sensitivity(::g_pFreeCamera->Sensitivity() + (yoffset * 0.01f));
	//	std::cout << "Mouse wheel: " << yoffset << std::endl;

	return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		::g_MouseLeftButtonIsDown = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		::g_MouseLeftButtonIsDown = false;
	}

	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		&& ::g_MouseIsInsideWindow)
	{
		::g_pFreeCamera->Yaw(::g_pFreeCamera->XChange(-x));

		::g_pFreeCamera->Pitch(::g_pFreeCamera->YChange(y));

	}
	else
	{
		::g_pFreeCamera->XChange(-x);
		::g_pFreeCamera->YChange(y);
	}

	// Adjust the mouse speed
	if (::g_MouseIsInsideWindow)
	{
		const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

	}


	// HACK 
	//::g_pFlyCamera->movementSpeed = 2.0f;

	return;
}//ProcessAsyncMouse(...


void ProcessAsyncKeys(GLFWwindow* window, double deltaTime)
{
	
	if (areAllModifiersUp(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			::g_pFreeCamera->MoveForward(1.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_S))
		{
			::g_pFreeCamera->MoveForward(-1.0f);
		}
		
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			::g_pFreeCamera->MoveRight(-1.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_D))
		{
			::g_pFreeCamera->MoveRight(1.0f);
		}
		
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			::g_pFreeCamera->MoveUp(-1.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			::g_pFreeCamera->MoveUp(1.0f);
		}
		
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		{
			::g_pFreeCamera->Speed(::g_pFreeCamera->Speed() - 0.1f);
		}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			::g_pFreeCamera->Speed(::g_pFreeCamera->Speed() + 0.1f);
		}
		cur += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			
			if (cur >= UPDATE_FREQ)
			{
				UserInputState uis;
				uis.input = UserInputState::FIRE;
				::g_client.Send(uis);
				cur = 0;
			}
		}
	}//if(AreAllModifiersUp(window)

	return;
}// ProcessAsyncKeys(..



void ShipControls(GLFWwindow* window, double deltaTime)
{

	UserInputState uis;
	uis.input = (UserInputState::Input) - 1;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		uis.input = UserInputState::FORWARD;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		uis.input = UserInputState::BACKWARD;
	}


	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pFindObjectByFriendlyName("Ship" + std::to_string(::g_client.GetId() + 1))->setAngularVelocity(glm::vec3(0.0f, SHIP_SPEED*2, 0.0f));
		uis.input = UserInputState::TURN_LEFT;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pFindObjectByFriendlyName("Ship" + std::to_string(::g_client.GetId() + 1))->setAngularVelocity(glm::vec3(0.0f, -SHIP_SPEED*2, 0.0f));
		uis.input = UserInputState::TURN_RIGHT;
	}
	else
	{
		if (pFindObjectByFriendlyName("Ship" + std::to_string(::g_client.GetId() + 1)))
			pFindObjectByFriendlyName("Ship" + std::to_string(::g_client.GetId() + 1))->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		uis.input = UserInputState::FIRE;
	}


	cur += deltaTime;
	if (cur >= UPDATE_FREQ)
	{
		::g_client.Send(uis);
		cur -= UPDATE_FREQ;
	}



	return;
}// ProcessAsyncKeys(..