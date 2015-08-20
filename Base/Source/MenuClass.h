#pragma once
#include "Vector2.h"

class CMenuClass
{
public:
	/*
		Different menu states
		Add more if needed
	*/
	enum MENU_STATE
	{
		PLAY_GAME = 1,
		CONTROLS,
		LEVEL_SELECTION,
		LOAD,
		LOSE_SCREEN,
		WIN_SCREEN,
		MAX_STATE,
	};
	enum LEVELS
	{
		LEVEL1 = 1,
		LEVEL2,
		LEVEL3,
		LEVEL4,
		LEVEL5,
		MAX_LEVEL,
	};

	CMenuClass(void);
	~CMenuClass(void);

	//Menu update
	int Update(double dt);
	void ControlMenu();
	void DefaultMenu();
	void LoadingMenu();
	int  LevelSelectMenu();
	void LostScreen();
	void WinScreen();

	//Get and Set functions
	float getCurrentSelectPos_X();
	float getCurrentSelectPos_Y();

	bool getControlState();
	
	bool getLoadingLevels();
	void setLoadingLevels(bool input);
	bool getChoosingLevels();
	int igetCurrentLevel(); 

	bool getLostState();
	void setLostState(bool input);
	bool getWinState();
	void setWinState(bool input);
	bool getMenuState();


	float getLevel1Size();
	float getLevel2Size();
	float getLevel3Size();
	float getLevel4Size();
	float getLevel5Size();

	int MenuStates();
	bool getReset();
	void setReset(bool input);

	void menuFeedback();
	void menuFeedback2();

	float inputDelay;	//limter for input delay
	float DELAY_RATE;	//Input delay rate
	float DELAY_RATE2;

private:
	int menuOption;	//Player's option
	int GameLevels;	//Game Level
	int LevelOption;

	Vector2 playPos;	//feedback pos
	Vector2 controlPos;	//feedback pos
	Vector2 loadPos;
	Vector2 levelPos;
	Vector2 currentSelectionPos;	//feedBackpos
		
	bool showMenu;	//Menu loop control
	bool showControls;	//Control menu
	bool Loading; //Loading menu
	bool LevelSelect; //Level Selecting Menu

	bool Win;	//Win state of player
	bool Lost;	//lose state of player
	bool Reset;	//Reset game

	float SELECTED_SIZE;
	float NORMAL_SIZE;
	float Level1Size;	//Text size
	float Level2Size;
	float Level3Size;
	float Level4Size;
	float Level5Size;
};

