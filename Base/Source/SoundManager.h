#pragma once
#include "irrKlang.h"
#include <iostream>

using std::cout;

using namespace irrklang;

class CSoundManager
{
public:
	/*
		Types of sounds used in game
	*/
	enum SOUND_TYPE	
	{
		GAME_BGM,
		SHURIKEN,
		SHURIKENSOUND,
		RIFLE,
		HEALTH,
		POWER_UP,
		COIN,
		MENU_FEEDBACK,
		SKELE_DEATH,
		GHOST_DEATH,
		GAMEOVER,
		LASER_OFF,
		MENU_BGM,
		ENEMY_BGM,
		HURT_BGM,
		MAX_SOUNDS,
	};
	CSoundManager(void);
	~CSoundManager(void);

	void Init();	//Creates sound engine

	void stopSound();	//Stops playing all sounds
	void playSounds(unsigned input);	//Plays sound based on input

private:
	ISoundEngine* engine;
	ISound* Sound;
};

