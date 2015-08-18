#include "MenuClass.h"
#include "Application.h"

CMenuClass::CMenuClass(void)
{
	menuOption = PLAY_GAME;
	showMenu = true;

	currentSelectionPos.x = 130.f;
	currentSelectionPos.y = 325.f;
	playPos.x = 450.f;	//Feedback position
	controlPos.x = 300.f;
	playPos.y = 325.f;	//Feedback position
	controlPos.y = 195.f;

	inputDelay = 0.f;
	DELAY_RATE = 0.2f;

	showControls = false;

	Win = false;
	Lost = false;
	Reset = false;
}
CMenuClass::~CMenuClass(void)
{
}

bool CMenuClass::getMenuState()
{
	return showMenu;
}

bool CMenuClass::getControlState()
{
	return showControls;
}

float CMenuClass::getCurrentSelectPos_X()
{
	return this->currentSelectionPos.x;
}
float CMenuClass::getCurrentSelectPos_Y()
{
	return this->currentSelectionPos.y;
}

bool CMenuClass::getLostState()
{
	return Lost;
}
void CMenuClass::setLostState(bool input)
{
	Lost = input;
}
bool CMenuClass::getWinState()
{
	return this->Win;
}
void CMenuClass::setWinState(bool input)
{
	this->Win = input;
}
bool CMenuClass::getReset()
{
	return Reset;
}
void CMenuClass::setReset(bool input)
{
	Reset = input;
}

void CMenuClass::menuFeedback()
{
	//==================================//
	//			Menu FeedBack			//
	//=================================//
	if(menuOption == PLAY_GAME)
	{
		currentSelectionPos.x = playPos.x;
		currentSelectionPos.y = playPos.y;
	}
	else if(menuOption == CONTROLS)
	{
		currentSelectionPos.x = controlPos.x;
		currentSelectionPos.y = controlPos.y;
	}
}

void CMenuClass::ControlMenu()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		showControls = false;
		inputDelay = 0.f;
	}
}
void CMenuClass::DefaultMenu()
{
	//==================================//
	//			Menu Navigation			//
	//=================================//
	if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE)
	{
		if(menuOption == PLAY_GAME)
		{
			menuOption = CONTROLS;
		}
		else if(menuOption == CONTROLS)
		{
			menuOption--;
		}

		inputDelay = 0.f;
	}

	if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE)
	{
		if(menuOption == PLAY_GAME)
		{
			menuOption++;
		}
		else if(menuOption == CONTROLS)
		{
			menuOption = PLAY_GAME;
		}

		inputDelay = 0.f;
	}

	//==================================//
	//			Menu Selection			//
	//=================================//
	if(Application::IsKeyPressed(VK_RETURN) && menuOption == PLAY_GAME  && inputDelay > DELAY_RATE)
	{
		showMenu = false;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && menuOption == CONTROLS  && inputDelay > DELAY_RATE)
	{
		showControls = true;
		inputDelay = 0.f;
	}

	menuFeedback();
}
void CMenuClass::LostScreen()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		showMenu = true;
		Lost = false;
		Reset = true;
		inputDelay = 0.f;
	}
}
void CMenuClass::WinScreen()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		showMenu = true;
		Win = false;
		inputDelay = 0.f;
	}
}

void CMenuClass::Update(double dt)
{
	inputDelay += (float)dt;
	
	//Display default menu
	if(showMenu == true && showControls == false)
	{
		DefaultMenu();
	}
	//Display Control menu
	else if(showMenu == true && showControls == true)
	{
		ControlMenu();
	}
	else if(Lost == true)
	{
		LostScreen();
	}
	else if(Win == true)
	{
		WinScreen();
	}
}