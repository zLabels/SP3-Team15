#include "MenuClass.h"
#include "Application.h"

CMenuClass::CMenuClass(void)
{
	GameLevels = LEVEL1;
	LevelOption = LEVEL1;
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
	DELAY_RATE2 = 0.2f;

	Level1Size = 3.f;	//Text size
	Level2Size = 3.f;
	Level3Size = 3.f;
	Level4Size = 3.f;
	Level5Size = 3.f;

	showControls = false;
	Loading = false;
	LevelSelect = false;

	Win = false;
	Lost = false;
	Reset = false;

	SELECTED_SIZE = 4.f;
	NORMAL_SIZE = 3.f;
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
	return this->LevelSelect;
}
int CMenuClass::igetCurrentLevel() 
{
	return this->GameLevels;
}
float CMenuClass::getLevel1Size()
{
	return this->Level1Size;
}
float CMenuClass::getLevel2Size()
{
	return this->Level2Size;
}
float CMenuClass::getLevel3Size()
{
	return this->Level3Size;
}
float CMenuClass::getLevel4Size()
{
	return this->Level4Size;
}
float CMenuClass::getLevel5Size()
{
	return this->Level5Size;
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

void CMenuClass::menuFeedback2()
{
	//==================================//
	//			Menu FeedBack			//
	//=================================//
	if(LevelOption == LEVEL1)
	{
		Level1Size = SELECTED_SIZE;
		Level2Size = NORMAL_SIZE;
		Level3Size = NORMAL_SIZE;
		Level4Size = NORMAL_SIZE;
		Level5Size = NORMAL_SIZE;
	}
	else if(LevelOption == LEVEL2)
	{
		Level1Size = NORMAL_SIZE;
		Level2Size = SELECTED_SIZE;
		Level3Size = NORMAL_SIZE;
		Level4Size = NORMAL_SIZE;
		Level5Size = NORMAL_SIZE;
	}
	else if(LevelOption == LEVEL3)
	{
		Level1Size = NORMAL_SIZE;
		Level2Size = NORMAL_SIZE;
		Level3Size = SELECTED_SIZE;
		Level4Size = NORMAL_SIZE;
		Level5Size = NORMAL_SIZE;
	}
	else if(LevelOption == LEVEL4)
	{
		Level1Size = NORMAL_SIZE;
		Level2Size = NORMAL_SIZE;
		Level3Size = NORMAL_SIZE;
		Level4Size = SELECTED_SIZE;
		Level5Size = NORMAL_SIZE;
	}
	else if(LevelOption == LEVEL5)
	{
		Level1Size = NORMAL_SIZE;
		Level2Size = NORMAL_SIZE;
		Level3Size = NORMAL_SIZE;
		Level4Size = NORMAL_SIZE;
		Level5Size = SELECTED_SIZE;
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

int CMenuClass::LevelSelectMenu()
{
	if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE2)
	{
		if(LevelOption == LEVEL1)
		{
			LevelOption = LEVEL5;
		}
		else if(LevelOption == LEVEL2)
		{
			LevelOption--;
		}
		else if(LevelOption == LEVEL3)
		{
			LevelOption--;
		}
		else if(LevelOption == LEVEL4)
		{
			LevelOption--;
		}
		else if(LevelOption == LEVEL5)
		{
			LevelOption--;
		}

		inputDelay = 0.f;
	}

	if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE2)
	{
		if(LevelOption == LEVEL1)
		{
			LevelOption++;
		}
		else if(LevelOption == LEVEL2)
		{
			LevelOption++;
		}
		else if(LevelOption == LEVEL3)
		{
			LevelOption++;
		}
		else if(LevelOption == LEVEL4)
		{
			LevelOption++;
		}
		else if(LevelOption == LEVEL5)
		{
			LevelOption = LEVEL1;
		}

		inputDelay = 0.f;
	}

	//==================================//
	//			Menu Selection			//
	//=================================//
	if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL1 && inputDelay > DELAY_RATE2)
	{
		LevelSelect = false;
		GameLevels = LEVEL1;
		inputDelay = 0.f;
		showMenu = false;
		cout <<"level";
		if(GameLevels != 0)
			return GameLevels;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL2 && inputDelay > DELAY_RATE2)
	{
		LevelSelect = false;
		GameLevels = LEVEL2;
		inputDelay = 0.f;
		showMenu = false;
		return GameLevels;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL3 && inputDelay > DELAY_RATE2)
	{
		LevelSelect = false;
		GameLevels = LEVEL3;
		inputDelay = 0.f;
		showMenu = false;
		return GameLevels;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL4 && inputDelay > DELAY_RATE2)
	{
		LevelSelect = false;
		GameLevels = LEVEL4;
		inputDelay = 0.f;
		showMenu = false;
		return GameLevels;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL5 && inputDelay > DELAY_RATE2)
	{
		LevelSelect = false;
		GameLevels = LEVEL5;
		inputDelay = 0.f;
		showMenu = false;
		return GameLevels;
	}
	menuFeedback2();

	return 0;
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
	//if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE3)
	//{
	//	if(menuOption == B_HEALTH)
	//	{
	//		menuOption = B_WEAPON;
	//	}
	//	else if(menuOption == B_WEAPON)
	//	{
	//		menuOption = B_HEALTH;
	//	}

	//	inputDelay = 0.f;
	//}

	//if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE3)
	//{
	//	if(menuOption == B_HEALTH)
	//	{
	//		menuOption = B_WEAPON;
	//	}
	//	else if(menuOption == B_WEAPON)
	//	{
	//		menuOption = B_HEALTH;
	//	}
	//	inputDelay = 0.f;
	//}
	//if(Application::IsKeyPressed(VK_RETURN) && menuOption == B_HEALTH  && inputDelay > DELAY_RATE3)
	//{
	//	cout<<"HEALTH ++"<<endl;
	//	inputDelay = 0.f;
	//}
	//else if(Application::IsKeyPressed(VK_RETURN) && menuOption == B_WEAPON  && inputDelay > DELAY_RATE3)
	//{
	//	cout<<"WEAPON ++"<<endl;
	//	inputDelay = 0.f;
	//}

	//menuFeedback3();
}

int CMenuClass::Update(double dt)
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
		int select = LevelSelectMenu();
		if(select != 0)
		{
			return select;
		}
	}
	else if(Lost == true)
	{
		LostScreen();
	}
	else if(Win == true)
	{
		WinScreen();
	}

	return 0;
}