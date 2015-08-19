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
	CMenuClass(void);
	~CMenuClass(void);

	//Menu update
	void Update(double dt);
	void ControlMenu();
	void DefaultMenu();
	void LoadingMenu();
	void LevelSelectMenu();
	void LostScreen();
	void WinScreen();

	//Get and Set functions
	float getCurrentSelectPos_X();
	float getCurrentSelectPos_Y();

	bool getControlState();
	
	bool getLoadingLevels();
	void setLoadingLevels(bool input);
	bool getChoosingLevels();
	//void setLevelChoosen(int);

	bool getLostState();
	void setLostState(bool input);
	bool getWinState();
	void setWinState(bool input);
	bool getMenuState();

	bool getReset();
	void setReset(bool input);

	void menuFeedback();

	float inputDelay;	//limter for input delay
	float DELAY_RATE;	//Input delay rate

private:
	int menuOption;	//Player's option
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
};

