#include "SoundManager.h"


CSoundManager::CSoundManager(void) :
			engine(NULL),
			Sound(NULL)
{
}

CSoundManager::~CSoundManager(void)
{
	if(Sound)
	{
		delete Sound;
		Sound = NULL;
	}
	if(engine)
	{
		delete engine;
		engine = NULL;
	}
}

void CSoundManager::Init()
{
	engine = createIrrKlangDevice();
	if (!engine)
	{
		cout << ("Could not startup engine\n");
	}
}

void CSoundManager::stopSound()
{
	engine->stopAllSounds();
}
void CSoundManager::playSounds(unsigned input)
{
	//Plays sound based on input
	switch(input)
	{
	case GAME_BGM:
		if(!engine->isCurrentlyPlaying("Sound\\Game\\escape.mp3"))
		{
			Sound = engine->play2D("Sound\\Game\\escape.mp3",true);
		}
		break;
	case SHURIKEN:
		Sound = engine->play2D("Sound\\Game\\shuriken.mp3");
		break;
	case RIFLE:
		if(!engine->isCurrentlyPlaying("Sound\\Game\\Rifle_sound.mp3"))
		{
			Sound = engine->play2D("Sound\\Game\\Rifle_sound.mp3");
		}
		break;
	case MENU_FEEDBACK:
		Sound = engine->play2D("Sound\\Menu\\MenuFeedback.mp3");
		break;
	case SKELE_DEATH:
		if(!engine->isCurrentlyPlaying("Sound\\SkeletonDeath.wav"))
		{
			Sound = engine->play2D("Sound\\SkeletonDeath.wav");
		}
		break;
	case GHOST_DEATH:
		if(!engine->isCurrentlyPlaying("Sound\\GhostDeath.wav"))
		{
			Sound = engine->play2D("Sound\\GhostDeath.wav");
		}
		break;

	}
}