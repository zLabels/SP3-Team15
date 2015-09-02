#include "MenuClass.h"
#include "Application.h"

CMenuClass::CMenuClass(void)
{
	GameLevels = LEVEL1;
	LevelOption = LEVEL1;
	menuOption = PLAY_GAME;
	ItemOption = B_HEALTH;
	showMenu = true;

	currentSelectionPos.x = 130.f;
	currentSelectionPos.y = 325.f;
	playPos.x = 450.f;	//Feedback position
	controlPos.x = 450.f;
	loadPos.x = 450.f;
	levelPos.x = 450.f;
	creditPos.x = 450.f;

	playPos.y = 335.f;	//Feedback position
	controlPos.y = 280.f;
	loadPos.y = 225.f;
	levelPos.y = 175.f;
	creditPos.y = 125.f;

	inputDelay = 0.f;
	DELAY_RATE = 0.2f;
	DELAY_RATE2 = 0.2f;
	DELAY_RATE3 = 0.2f;
	DELAY_RATE4 = 0.3f;
	DELAY_RATE5 = 0.2f;
	//Level1Size = 3.f;	//Text size
	//Level2Size = 3.f;
	//Level3Size = 3.f;
	//Level4Size = 3.f;
	//Level5Size = 3.f;

	showCredits = false;
	showControls = false;
	Loading = false;
	LevelSelect = false;
	Pause = false;

	BuyHealth = false;
	BuyWeapon = false;
	Health = false;
	Weapon = false;

	Win = false;
	Lost = false;
	LosingSound = false;
	Reset = false;
		
	save = false;
	save_n_quit = false;
	quit = false;

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

bool CMenuClass::getCreditsState()
{
	return showCredits;
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

float CMenuClass::getHealthSize()
{
	return this->HealthSize;
}
float CMenuClass::getWeaponSize()
{
	return this->WeaponSize;
}
float CMenuClass::getOutSize()
{
	return this->OutSize;
}

float CMenuClass::getSaveSize()
{
	return this->SaveSize;
}
float CMenuClass::getQuitSize()
{
	return this->QuitSize;
}
float CMenuClass::getSQSize()
{
	return this->SQ_Size;
}
float CMenuClass::getReturnSize()
{
	return this->R_Size;
}
float CMenuClass::getReturnMainSize()
{
	return this->RMain_Size;
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
bool CMenuClass::getLostSound()
{
	return LosingSound; 
}
void CMenuClass::setLostSound(bool input)
{
	LosingSound = input;
}

bool CMenuClass::getWinState()
{
	return this->Win;
}
void CMenuClass::setWinState(bool input)
{
	this->Win = input;
}

void CMenuClass::setPauseState(bool input)
{
	this->Pause = input;
}
bool CMenuClass::getPauseState()
{
	return this->Pause;
}

bool CMenuClass::getSave()
{	
	return this->save;
}
void CMenuClass::setSave(bool input)
{
	this->save = input;
}
bool CMenuClass::getSavenQuit()
{
	return this->save_n_quit;
}
void CMenuClass::setSavenQuit(bool input)
{
	this->save_n_quit = input;
}
bool CMenuClass::getQuit()
{
	return this->quit;
}
void CMenuClass::setQuit(bool input)
{
	this->quit = input;
}

bool CMenuClass::getReset()
{
	return Reset;
}
void CMenuClass::setReset(bool input)
{
	Reset = input;
}

bool CMenuClass::getHealthBought()
{
	//AddHealth();
	return this->BuyHealth;
}

void CMenuClass::setHealthBought(bool input)
{
	this->BuyHealth = input;
}

bool CMenuClass::getWeaponBought()
{
	//AddWeapon();
	return this->BuyWeapon;
}

void CMenuClass::setWeaponBought(bool input)
{
	this->BuyWeapon = input;
}

bool CMenuClass::AddHealth(int money)
{
	Money.SetCash(money); 
	if(Money.GetCash() >= 5)
	{
		return true;
		//Money.SetCash(money-5);
	}
	return false;
}
bool CMenuClass::AddWeapon(int money)
{
	Money.SetCash(money); 
	if(Money.GetCash() >= 10)
	{
		return true;
		//Money.SetCash(money-10);
	}
	return false;
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
	else if(menuOption == CREDITS)
	{
		currentSelectionPos.x = creditPos.x;
		currentSelectionPos.y = creditPos.y;
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

	
void CMenuClass::CreditsMenu()
{
	if(Application::IsKeyPressed(VK_RETURN)  && inputDelay > DELAY_RATE)
	{
		showCredits  = false;
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
	if(Application::IsKeyPressed(VK_LEFT) && inputDelay > DELAY_RATE2)
	{
		if(LevelOption == LEVEL1)
		{
			LevelOption = LEVEL3;
		}
		else if(LevelOption == LEVEL3)
		{
			LevelOption = LEVEL1;
		}
		else if(LevelOption == LEVEL2)
		{
			LevelOption = LEVEL4;
		}
		else if(LevelOption == LEVEL4)
		{
			LevelOption = LEVEL2;
		}
		inputDelay = 0.f;
	}
	if(Application::IsKeyPressed(VK_RIGHT) && inputDelay > DELAY_RATE2)
	{
		if(LevelOption == LEVEL1)
		{
			LevelOption = LEVEL3;
		}
		else if(LevelOption == LEVEL3)
		{
			LevelOption = LEVEL1;
		}
		else if(LevelOption == LEVEL2)
		{
			LevelOption = LEVEL4;
		}
		else if(LevelOption == LEVEL4)
		{
			LevelOption = LEVEL2;
		}
		inputDelay = 0.f;
	}
	if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE2)
	{
		if(LevelOption == LEVEL1)
		{
			LevelOption = LEVEL4;
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
		//else if(LevelOption == LEVEL5)
		//{
		//	LevelOption--;
		//}

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
			LevelOption = LEVEL1;
		}
		//else if(LevelOption == LEVEL5)
		//{
		//	LevelOption = LEVEL1;
		//}

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
	//else if(Application::IsKeyPressed(VK_RETURN) && LevelOption == LEVEL5 && inputDelay > DELAY_RATE2)
	//{
	//	LevelSelect = false;
	//	GameLevels = LEVEL5;
	//	inputDelay = 0.f;
	//	showMenu = false;
	//	return GameLevels;
	//}
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
			menuOption = CREDITS;
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
		else if(menuOption == CREDITS)
		{
			menuOption = LEVEL_SELECTION;
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
			menuOption = CREDITS;
		}
		else if(menuOption == CREDITS)
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
	else if(Application::IsKeyPressed(VK_RETURN) && menuOption == CREDITS  && inputDelay > DELAY_RATE)
	{
		showCredits = true;
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
	if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE3)
	{
		if(ItemOption == B_HEALTH)
		{
			ItemOption = B_WEAPON;
		}
		else if(ItemOption == B_WEAPON)
		{
			ItemOption = B_OUT;
		}
		else if(ItemOption == B_OUT)
		{
			ItemOption = B_HEALTH;
		}

		inputDelay = 0.f;
	}

	if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE3)
	{
		if(ItemOption == B_HEALTH)
		{
			ItemOption = B_OUT;
		}
		else if(ItemOption == B_WEAPON)
		{
			ItemOption = B_HEALTH;
		}
		else if(ItemOption == B_OUT)
		{
			ItemOption = B_WEAPON;
		}
		inputDelay = 0.f;
	}
	if(Application::IsKeyPressed(VK_RETURN) && ItemOption == B_HEALTH  && inputDelay > DELAY_RATE3)
	{
		//cout<<"HEALTH ++"<<endl;
		BuyHealth = true;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == B_WEAPON  && inputDelay > DELAY_RATE3)
	{
		//cout<<"WEAPON ++"<<endl;
		BuyWeapon = true;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == B_OUT  && inputDelay > DELAY_RATE3)
	{
		Win = false;
		inputDelay = 0.f;
	}


	if(ItemOption == B_HEALTH)
	{
		HealthSize = SELECTED_SIZE;
		WeaponSize = NORMAL_SIZE;
		OutSize = NORMAL_SIZE;
	}
	else if(ItemOption == B_WEAPON)
	{
		HealthSize = NORMAL_SIZE;
		WeaponSize = SELECTED_SIZE;
		OutSize = NORMAL_SIZE;
	}
	else if(ItemOption == B_OUT)
	{
		HealthSize = NORMAL_SIZE;
		WeaponSize = NORMAL_SIZE;
		OutSize = SELECTED_SIZE;
	}
}

void CMenuClass::PauseMenu()
{
	if(Application::IsKeyPressed(VK_DOWN) && inputDelay > DELAY_RATE4)
	{
		if(ItemOption == SAVE)
		{
			ItemOption = SAVE_N_QUIT;
		}
		else if(ItemOption == SAVE_N_QUIT)
		{
			ItemOption = QUIT;
		}
		else if(ItemOption == QUIT)
		{
			ItemOption = RETURN;
		}
		else if(ItemOption == RETURN)
		{
			ItemOption = RETURN_MAIN;
		}
		else if(ItemOption == RETURN_MAIN)
		{
			ItemOption = SAVE;
		}
		inputDelay = 0.f;
	}

	if(Application::IsKeyPressed(VK_UP) && inputDelay > DELAY_RATE4)
	{
		if(ItemOption == SAVE)
		{
			ItemOption = RETURN_MAIN;
		}
		else if(ItemOption == SAVE_N_QUIT)
		{
			ItemOption = SAVE;
		}
		else if(ItemOption == QUIT)
		{
			ItemOption = SAVE_N_QUIT;
		}
		else if(ItemOption == RETURN)
		{
			ItemOption = QUIT;
		}
		else if(ItemOption == RETURN_MAIN)
		{
			ItemOption = RETURN;
		}
		inputDelay = 0.f;
	}
	if(Application::IsKeyPressed(VK_RETURN) && ItemOption == SAVE  && inputDelay > DELAY_RATE5)
	{
		save = true;
		inputDelay = 0.2f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == SAVE_N_QUIT  && inputDelay > DELAY_RATE4)
	{
		save_n_quit = true;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == QUIT  && inputDelay > DELAY_RATE4)
	{
		quit = true;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == RETURN  && inputDelay > DELAY_RATE4)
	{
		Pause = false;
		inputDelay = 0.f;
	}
	else if(Application::IsKeyPressed(VK_RETURN) && ItemOption == RETURN_MAIN  && inputDelay > DELAY_RATE4)
	{
		showMenu = true;
		Pause = false;
		Reset = true;
		inputDelay = 0.f;
	}

	if(ItemOption == SAVE)
	{
		SaveSize = SELECTED_SIZE;
		SQ_Size = NORMAL_SIZE;
		QuitSize = NORMAL_SIZE;
		R_Size = NORMAL_SIZE;
		RMain_Size = NORMAL_SIZE;
	}
	else if(ItemOption == SAVE_N_QUIT)
	{
		SaveSize = NORMAL_SIZE;
		SQ_Size = SELECTED_SIZE;
		QuitSize = NORMAL_SIZE;
		R_Size = NORMAL_SIZE;
		RMain_Size = NORMAL_SIZE;
	}
	else if(ItemOption == QUIT)
	{
		SaveSize = NORMAL_SIZE;
		SQ_Size = NORMAL_SIZE;
		QuitSize = SELECTED_SIZE;
		R_Size = NORMAL_SIZE;
		RMain_Size = NORMAL_SIZE;
	}
	else if(ItemOption == RETURN)
	{
		SaveSize = NORMAL_SIZE;
		SQ_Size = NORMAL_SIZE;
		QuitSize = NORMAL_SIZE;
		R_Size = SELECTED_SIZE;
		RMain_Size = NORMAL_SIZE;
	}
	else if(ItemOption == RETURN_MAIN)
	{
		SaveSize = NORMAL_SIZE;
		SQ_Size = NORMAL_SIZE;
		QuitSize = NORMAL_SIZE;
		R_Size = NORMAL_SIZE;
		RMain_Size = SELECTED_SIZE;
	}
}


int CMenuClass::Update(double dt)
{
	inputDelay += (float)dt;
	
	//Display default menu
	if(showMenu == true && showControls == false && Loading == false && LevelSelect == false && showCredits == false)
	{
		DefaultMenu();
	}
	//Display Control menu
	else if(showMenu == true && showControls == true && Loading == false && LevelSelect == false && showCredits == false)
	{
		ControlMenu();
	}
	else if(showMenu == true && showControls == false && Loading == false && LevelSelect == false && showCredits == true)
	{
		CreditsMenu();
	}
	else if(showMenu == true && Loading == true && showControls == false && LevelSelect == false && showCredits == false)
	{
		LoadingMenu();
	}
	else if(showMenu == true && LevelSelect == true && Loading == false && showControls == false && showCredits == false)
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
		Pause = false;
		Win = false;
	}
	else if(showMenu == false && Win == true && Pause == false)
	{
		WinScreen();
		Pause = false;
	}
	else if(showMenu == false && Pause == true && Win == false)
	{
		PauseMenu();
		Win = false;
	}

	return 0;
}