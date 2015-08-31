/******************************************************************************/
/*!
\file	CMenuClass.h
\author Princeton Chew, Jessica Teo
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Menu Class
*/
/******************************************************************************/
#pragma once
#include "Vector2.h"
#include "Shop.h"
#include "Hero.h"

/******************************************************************************/
/*!
		Class CMenuClass
\brief
Defines the variables and functions required for the menu in game.

*/
/******************************************************************************/
class CMenuClass
{
public:
	/*
	Different menu states
	*/
	enum MENU_STATE
	{
		PLAY_GAME = 1,
		CONTROLS,
		LEVEL_SELECTION,
		LOAD,
		LOSE_SCREEN,
		WIN_SCREEN,
		PAUSE,
		MAX_STATE,
	};
	/*
	Enum for different levels
	*/
	enum LEVELS
	{
		LEVEL1 = 1,
		LEVEL2,
		LEVEL3,
		LEVEL4,
		LEVEL5,
		MAX_LEVEL,
	};
	/*
	Enum for different buy options
	*/
	enum BUY
	{
		B_HEALTH = 1,
		B_WEAPON,
		B_OUT,
		MAX_THINGS,
	};
	/*
	Enum for pause state
	*/
	enum PAUSE_MENU
	{
		SAVE,
		SAVE_N_QUIT,
		QUIT,
		RETURN,
		RETURN_MAIN,
		MAX_PAUSE_MENU,
	};

	CMenuClass(void);	//Default constructor
	~CMenuClass(void);	//Default destructor

	int Update(double dt);	//Update menu
	void ControlMenu();	//Control menu
	void DefaultMenu();	//Default menu
	void LoadingMenu();	//Loading menu
	int LevelSelectMenu();	//Level selection menu
	void LostScreen();	//Lose screen
	void WinScreen();	//Win Screen
	void PauseMenu();	//Pause menu

	float getCurrentSelectPos_X();	//Get current selection x position
	float getCurrentSelectPos_Y();	//Get current selection y position

	bool getControlState();	//Get control menu status

	bool getLoadingLevels();	//Get loading level menu status
	void setLoadingLevels(bool input);	//Set loading level menu status
	bool getChoosingLevels();	//Get choosing levels
	int igetCurrentLevel();		//Get current level

	bool getLostState();	//Get lost state of game
	void setLostState(bool input);	//Set lost status of game
	bool getWinState();	//Get win state of game
	void setWinState(bool input);	//Set win status of game
	bool getMenuState();	//Get the state of menu

	void setPauseState(bool input);	//Set pause menu state
	bool getPauseState();	//Get pause menu state

	bool getSave();	//Get save menu option
	void setSave(bool input);	//Set save menu state
	bool getSavenQuit();	//Get save and quit menu option
	void setSavenQuit(bool input);	//Set save and quit menu option
	bool getQuit();	//Get quit status of menu
	void setQuit(bool input);	//Set quit status of menu

	bool getHealthBought();	//Get health bought status
	void setHealthBought(bool input);	//Set health bought status
	bool getWeaponBought();	//Get weapon bought status
	void setWeaponBought(bool input);	//Set weapon bought status

	bool AddHealth(int);	//Adding health to player
	bool AddWeapon(int);	//Adding weapon to player

	bool getLostSound();	//Get lost sound of menu
	void setLostSound(bool input);	//set lost sound of menu

	float getLevel1Size();	//Get level 1 text size
	float getLevel2Size();	//Get level 2 text size
	float getLevel3Size();	//Get level 3 text size
	float getLevel4Size();	//Get level 4 text size
	float getLevel5Size();	//Get level 5 text size

	float getHealthSize();	//Get health text size
	float getWeaponSize();	//Get weapon text size
	float getOutSize();	//Get out text size

	float getSaveSize();	//Get save text size
	float getQuitSize();	//Get quit text size
	float getSQSize();	//Get save n quit text size
	float getReturnSize();	//Get return text size
	float getReturnMainSize();	//Get return main text size

	int MenuStates();	//Get menu states
	bool getReset();	//Get reset state of game
	void setReset(bool input);	//Set reset state of game

	void menuFeedback();	//Feed back for menu
	void menuFeedback2();	//Feed back for menu

	float inputDelay;	//limter for input delay
	float DELAY_RATE;	//Input delay rate
	float DELAY_RATE2;	//Input delay rate
	float DELAY_RATE3;	//Input delay rate
	float DELAY_RATE4;	//Input delay rate

private:
	CShop Money;

	int menuOption;	//Player's option
	int GameLevels;	//Game Level
	int LevelOption;
	int ItemOption;

	Vector2 playPos;	//feedback pos
	Vector2 controlPos;	//feedback pos
	Vector2 loadPos;
	Vector2 levelPos;
	Vector2 currentSelectionPos;	//feedBackpos

	bool save;
	bool save_n_quit;
	bool quit;

	bool BuyHealth;
	bool BuyWeapon;
	bool Health;
	bool Weapon;

	bool showMenu;	//Menu loop control
	bool showControls;	//Control menu
	bool Loading; //Loading menu
	bool LevelSelect; //Level Selecting Menu

	bool Win;	//Win state of player
	bool Lost;	//lose state of player
	bool Pause;

	bool LosingSound;
	bool Reset;	//Reset game

	float SELECTED_SIZE;
	float NORMAL_SIZE;
	float Level1Size;	//Text size
	float Level2Size;
	float Level3Size;
	float Level4Size;
	float Level5Size;
	float HealthSize;
	float WeaponSize;
	float OutSize;
	float SaveSize;
	float QuitSize;
	float SQ_Size;
	float R_Size;
	float RMain_Size;
};

