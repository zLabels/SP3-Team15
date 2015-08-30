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
		if(!engine->isCurrentlyPlaying("Sound\\Game\\escape.wav"))
		{
			Sound = engine->play2D("Sound\\Game\\escape.wav",true);
		}
		break;
	case SHURIKEN:
		Sound = engine->play2D("Sound\\Game\\shuriken2.wav");
		break;
	case SHURIKENSOUND:
		Sound = engine->play2D("Sound\\Game\\CollectShuriken.wav");
		break;
	case RIFLE:
		Sound = engine->play2D("Sound\\Game\\Rifle_sound.wav");
		break;
	case HEALTH:
		Sound = engine->play2D("Sound\\Game\\Health.wav");
		break;
	case POWER_UP:
		Sound = engine->play2D("Sound\\Game\\powerup.wav");
		break;
	case COIN:
		Sound = engine->play2D("Sound\\Game\\coin.wav");
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
	case GAMEOVER:
		if(!engine->isCurrentlyPlaying("Sound\\Game\\GameOver.wav"))
		{
			Sound = engine->play2D("Sound\\Game\\GameOver.wav");
			break;
		}
	case LASER_OFF:
		//Sound = engine->play2D("Sound\\Game\\MenuFeedback.mp3");
		break;
	case MENU_BGM:
		if(!engine->isCurrentlyPlaying("Sound\\Game\\EPIC.wav"))
		{
			Sound = engine->play2D("Sound\\Game\\EPIC.wav");
			break;
		}
	case ENEMY_BGM:
		//Sound = engine->play2D("Sound\\Game\\MenuFeedback.mp3");
		break;	
	case HURT_BGM:
		//Sound = engine->play2D("Sound\\Game\\MenuFeedback.mp3");
		break;

	}
}