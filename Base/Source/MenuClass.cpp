#include "MenuClass.h"
#include "Application.h"

CMenuClass::CMenuClass(void)
{
	menuOption = PLAY_GAME;
	showMenu = true;

	currentSelectionPos.x = 130.f;
	currentSelectionPos.y = 325.f;
	playPos.x = 450.f;	//Feedback position
	controlPos.x = 450.f;
	loadPos.x = 450.f;
	levelPos.x = 450.f;

	playPos.y = 325.f;	//Feedback position
	controlPos.y = 270.f;
	loadPos.y = 215.f;
	levelPos.y = 160.f;

	inputDelay = 0.f;
	DELAY_RATE = 0.2f;

	showControls = false;
	Loading = false;
	LevelSelect = false;

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

bool CMenuClass::getLoadingLevels()
{
	return Loading;
}
void CMenuClass::setLoadingLevels(bool input)
{
	Loading = input;
}
bool CMenuClass::getChoosingLevels()
{
	return LevelSelect;
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
	else if(menuOption == LOAD)
	{
		currentSelectionPos.x = loadPos.x;
		currentSelectionPos.y = loadPos.y;
	}
	else if(menuOption == LEVEL_SELECTION)
	{
		currentSelectionPos.x = levelPos.x;
		currentSelectionPos.y = levelPos.y;
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

void CMenuClass::LoadingMenu()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		Loading = false;
		inputDelay = 0.f;
	}
}

void CMenuClass::LevelSelectMenu()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		LevelSelect = false;
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
			menuOption = LEVEL_SELECTION;
		}
		else if(menuOption == CONTROLS)
		{
			menuOption = PLAY_GAME;
		}
		else if(menuOption == LOAD)
		{
			menuOption = CONTROLS;
		}
		else if(menuOption == LEVEL_SELECTION)
		{
			menuOption = LOAD;
		}
		inputDelay = 0.f;
	}

	if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE)
	{
		if(menuOption == PLAY_GAME)
		{
			menuOption = CONTROLS;
		}
		else if(menuOption == CONTROLS)
		{
			menuOption = LOAD;
		}
		else if(menuOption == LOAD)
		{
			menuOption = LEVEL_SELECTION;
		}
		else if(menuOption == LEVEL_SELECTION)
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
	else if(Application::IsKeyPressed(VK_RETURN) && menuOption == LOAD  && inputDelay > DELAY_RATE)
	{
		showMenu = false;
		Loading = true;
		
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && menuOption == LEVEL_SELECTION  && inputDelay > DELAY_RATE)
	{
		LevelSelect = true;
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
	if(showMenu == true && showControls == false && Loading == false && LevelSelect == false)
	{
		DefaultMenu();
	}
	//Display Control menu
	else if(showMenu == true && showControls == true && Loading == false && LevelSelect == false)
	{
		ControlMenu();
	}
	else if(showMenu == true && Loading == true && showControls == false && LevelSelect == false)
	{
		LoadingMenu();
	}
	else if(showMenu == true && LevelSelect == true && Loading == false && showControls == false)
	{
		LevelSelectMenu();
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