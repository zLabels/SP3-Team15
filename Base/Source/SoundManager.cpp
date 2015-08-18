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
		if(!engine->isCurrentlyPlaying("Sound\\BGM.wav"))
		{
			Sound = engine->play2D("Sound\\BGM.wav",true);
		}
		break;
	case SABER_SWING:
		Sound = engine->play2D("Sound\\Saber\\Sword_swing.wav");
		break;
	case SABER_SHOCKWAVE:	
		Sound = engine->play2D("Sound\\Saber\\Saber_shockwave.wav");
		break;
	case SWORD_IMPACT:
		Sound = engine->play2D("Sound\\Saber\\Sword_impact.wav");
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