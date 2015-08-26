#include "Hero.h"
#include "TileCollision.h"

CHero* CHero::heroInstance = 0;

CHero::CHero(void):
	hero_inMidAir_Up(false),
	hero_inMidAir_Down(false),
	jumpspeed(0),
	jumpcount(0),
	hero_face_left(false),
	hero_face_right(true),
	hero_grappling(false),
    hero_invisible(false),
	hero_HP(100),
	hero_EP(100),
	hero_MS(3.f),
	kb_MS(10.f),
	mapOffset_X(0), 
	mapOffset_Y(0),
	mapFineOffset_X(0),
	mapFineOffset_Y(0),
	MAX_FALLSPEED(14),
	hero_damaged(false),
	kb_direction(false),
	invul_frame(0.f),
	MAX_INVUL(1.f),
    f_Current_Acceleration(0),
    hero_mass(4),
    hero_velocity(0),
    hero_maxspeed(3),
    f_force(1),
    hero_SCORE(0)
{
}
CHero::~CHero(void)
{
	while(hero_invent.getShurikenList().size() > 0)
	{
		CShuriken *star = hero_invent.getShurikenList().back();
		delete star;
		hero_invent.getShurikenList().pop_back();
	}
}

void CHero::HeroInit(float posX,float posY)
{
	this->HeroPos.x = posX;
	this->HeroPos.y = posY;

    
}
/********************************************************************************
Hero Jump
********************************************************************************/
void CHero::HeroJump()
{
	if (jumpcount < 3)
	{
		if(hero_inMidAir_Down == false)
		{
			hero_inMidAir_Up = true;
			hero_inMidAir_Down = false;
			jumpspeed = 14;
		}
		else if(jumpcount == 2)
		{
			hero_inMidAir_Up = true;
			hero_inMidAir_Down = false;
			jumpspeed = 14;
		}
	}
}
/********************************************************************************
Hero Move Up Down
********************************************************************************/
void CHero::HeroMoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{
		HeroPos.y = HeroPos.y + (int) (hero_MS * timeDiff);
	}
	else
	{
		HeroPos.y = HeroPos.y - (int) (hero_MS * timeDiff);
	}
}
/********************************************************************************
Hero Move Left Right
********************************************************************************/
void CHero::HeroMoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		tempHeroPos.x = HeroPos.x;
		 Hero_Acceleration(true, timeDiff);  //Acceleration
	}
	else
	{
		tempHeroPos.x = HeroPos.x;
		Hero_Acceleration(false, timeDiff); //Acceleration
	}
}

bool CHero::HeroGrapple(CMap* m_cMap,Vector3 direction,Vector3 hookPosition)
{
	hero_grappling = true;

	direction.Normalize();

	tempHeroPos.x = HeroPos.x;
	tempHeroPos.y = HeroPos.y;
	HeroPos.x += direction.x * 5.f;
	HeroPos.y += direction.y * 5.f;
	if( HeroCollisionCheck(m_cMap,true,true,true,true,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
	{
		HeroPos.x = tempHeroPos.x;
		HeroPos.y = tempHeroPos.y;
		hero_grappling = false;
		return true;
	}
	
	return false;
}

void CHero::HeroUseGrapple(Mesh* ptr,float target_x,float target_y)
{
	hero_invent.getGrapple().setData(ptr,HeroPos.x + 25.f,HeroPos.y + 40.f,
			0,target_x - (HeroPos.x + this->mapOffset_X + 25.f),target_y - (HeroPos.y + 40.f),true);
	hero_invent.getGrapple().setRotation(Math::RadianToDegree( -atan2(  ( target_x - (HeroPos.x + this->mapOffset_X + 25.f) ),(target_y - (HeroPos.y + 40.f) ) )));
	hero_invent.getGrapple().setActive(true);
}
void CHero::HeroThrowShuriken(Mesh* ptr,float target_x,float target_y)
{
	if( hero_invent.getShurikenCount() > 0 )
	{
		CShuriken* shuriken = hero_invent.FetchShuriken();
		shuriken->setData(ptr,HeroPos.x + 25.f,HeroPos.y + 40.f,
			100,target_x - (HeroPos.x + this->mapOffset_X + 25.f),target_y - (HeroPos.y + 40.f),true);
	}
}
/********************************************************************************
Hero Damaged by colliding with enemy
********************************************************************************/
void CHero::HeroDamaged(int enemy_x, int enemy_y)
{
	if( ((enemy_x - (HeroPos.x + mapOffset_X)) * (enemy_x - (HeroPos.x + mapOffset_X)) + (enemy_y - HeroPos.y) * (enemy_y - HeroPos.y)) < 625)	//Checking if enemy is within attack range
	{
		if(hero_damaged == false && invul_frame == 0)
		{
			hero_HP -= 5;
			hero_damaged = true;
			knockbackPos.x = 0;
			if(hero_face_left)
			{
				kb_direction = false;
			}
			else if(hero_face_right)
			{
				kb_direction = true;
			}
			invul_frame = MAX_INVUL;
		}
	}
}
/********************************************************************************
Hero Damaged by enemy
********************************************************************************/
void CHero::HeroDamaged(int Damage)
{
	if(hero_damaged == false && invul_frame == 0)
	{
		hero_HP -= Damage;
		hero_damaged = true;
		knockbackPos.x = 0;
		if(hero_face_left)
		{
			kb_direction = false;
		}
		else if(hero_face_right)
		{
			kb_direction = true;
		}
		invul_frame = MAX_INVUL;
	}
}
/********************************************************************************
Hero knocked back by damaged
********************************************************************************/
void CHero::HeroKnockBack(CMap* m_cMap)
{
	if(hero_damaged == true)
	{
		if(kb_direction == false)
		{
			tempHeroPos.x = HeroPos.x;
			knockbackPos.x += (int) (kb_MS);
			HeroPos.x += (int) (kb_MS);
			if (HeroPos.x < 0)
			{
				HeroPos.x = tempHeroPos.x;
			}
			//Collision checks to see if hero can be knocked back further
			//Right
			if ( HeroCollisionCheck(m_cMap,false,true,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
			{
				// Since the new position does not allow the hero to move into, then go back to the old position
				HeroPos.x = ((int) (HeroPos.x / m_cMap->GetTileSize())) * m_cMap->GetTileSize();	
				hero_damaged = false;
			}
			//Left
			else if( HeroCollisionCheck(m_cMap,true,false,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
			{
				HeroPos.x = tempHeroPos.x;
				hero_damaged = false;
			}
			else if( knockbackPos.x > 25 )
			{
				hero_damaged = false;
			}
		}
		else if(kb_direction == true)
		{
			tempHeroPos.x = HeroPos.x;
			knockbackPos.x -= (int) (kb_MS);
			HeroPos.x -= (int) (kb_MS);
			if (HeroPos.x < 0)
			{
				HeroPos.x = tempHeroPos.x;
			}
			//Collision checks to see if hero can be knocked back further
			//Right
			if ( HeroCollisionCheck(m_cMap,false,true,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
			{
				// Since the new position does not allow the hero to move into, then go back to the old position
				HeroPos.x = ((int) (HeroPos.x / m_cMap->GetTileSize())) * m_cMap->GetTileSize();	
				hero_damaged = false;
			}
			//Left
			else if( HeroCollisionCheck(m_cMap,true,false,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
			{
				HeroPos.x = tempHeroPos.x;
				hero_damaged = false;
			}
			else if( knockbackPos.x < -25 )
			{
				hero_damaged = false;
			}
		}
	}
}
/********************************************************************************
 Constrain the position of the Hero to within the border
 ********************************************************************************/
int CHero::ConstrainHeroX(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff,int mapWidth, int mapHeight,unsigned maplevel)
{
	if (HeroPos.x < leftBorder)
	{
		mapOffset_X =  mapOffset_X - (int) (hero_MS * timeDiff);
		if (mapOffset_X < 0)
		{
			mapOffset_X = 0;
		}
		else
		{
			HeroPos.x = leftBorder;
		}
	}
	else if (HeroPos.x > rightBorder)
	{
		mapOffset_X =  mapOffset_X + (int) (hero_MS * timeDiff);
		if (mapOffset_X > mapWidth)	// This must be changed to soft-coded
		{
			mapOffset_X = mapWidth;
		}
		else
		{
			HeroPos.x = rightBorder;
		}
	}

	if (HeroPos.x < 0)
	{
		HeroPos.x = tempHeroPos.x;
	}
	else if (mapOffset_X > mapWidth)
	{
		HeroPos.x = tempHeroPos.x;
	}
	return mapOffset_X;
}

int CHero::ConstrainHeroY(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, 
						float timeDiff,int screenWidth, int screenHeight,unsigned maplevel)
{
	return mapOffset_Y;
}

void CHero::Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel)
{
	//Right
	if ( HeroCollisionCheck(m_cMap,false,true,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
	{
		// Since the new position does not allow the hero to move into, then go back to the old position
		HeroPos.x = ((int) (HeroPos.x / m_cMap->GetTileSize())) * m_cMap->GetTileSize();	
	}
	//Left
	else if( HeroCollisionCheck(m_cMap,true,false,false,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true)
	{
		HeroPos.x = tempHeroPos.x;
	}
	//Falling
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false && hero_grappling == false)
	{
		//falling
		if ( HeroCollisionCheck(m_cMap,false,false,false,true,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == false )
		{
			hero_inMidAir_Down = true;
		}
	}

	// Update Hero's info
  	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
	}
	if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
	{
		tempHeroPos.y = HeroPos.y;
		// Check if the hero can move up into mid air...
		if ( HeroCollisionCheck(m_cMap,false,false,true,false,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true )
		{
			//// Since the new position does not allow the hero to move into, then go back to the old position
			//HeroPos.y = ((int) (HeroPos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			//hero_inMidAir_Up = false;
			//jumpspeed = 0;
			HeroPos.y = tempHeroPos.y;
			jumpspeed = 0;
			hero_inMidAir_Down = true;
			hero_inMidAir_Up = false;
		}
		else
		{
			HeroPos.y += jumpspeed;
			jumpspeed -= 1;
			if (jumpspeed == 0)
			{
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
			}
		}
	}
	else if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
	{
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((mapOffset_X+HeroPos.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
			checkPosition_X = 0;
		if (checkPosition_X > m_cMap->getNumOfTiles_MapWidth())
			checkPosition_X = m_cMap->getNumOfTiles_MapWidth();
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (HeroPos.y - jumpspeed) / m_cMap->GetTileSize());
		if (checkPosition_Y < 0)
			checkPosition_Y = 0;
		if (checkPosition_Y > m_cMap->GetNumOfTiles_Height())
			checkPosition_Y = m_cMap->GetNumOfTiles_Height();

		if ( HeroCollisionCheck(m_cMap,false,false,false,true,HeroPos.x,HeroPos.y,mapOffset_X,jumpspeed) == true )
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			HeroPos.y = ((int) (HeroPos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Down = false;
			jumpspeed = 0;
			jumpcount = 0;
		}
		else
		{
			HeroPos.y -= jumpspeed;
			jumpspeed += 1;
			if(jumpspeed > MAX_FALLSPEED)
			{
				jumpspeed = MAX_FALLSPEED;
			}
		}
	}

    //****************************************//
    //                                        //
    //             TILE COLLISION             //
    //                                        //
    //****************************************//
    if(HeroTileCheck(m_cMap, TILE_LASER,true,true,true,true,GetHeroPos_x(),GetHeroPos_y(),GetMapOffset_x(),Getjumpspeed()) == true)
    {
        hero_HP -= 1; // Minus health for touching the lasers.
    }

    if(HeroTileCheck(m_cMap, TILE_STEALTH_BOX,true,true,false,false,GetHeroPos_x(),GetHeroPos_y(),GetMapOffset_x(),Getjumpspeed()) == true)
    {
        hero_invisible = true;
    }
    else
    {
        hero_invisible = false;
    }
    
    if(HeroTileCheck(m_cMap, TILE_HEALTH,true,true,true,true,GetHeroPos_x(),GetHeroPos_y(),GetMapOffset_x(),Getjumpspeed()) == true)
    {
        float tempHeroPos_x = CHero::GetInstance()->GetHeroPos_x();	//Hero current position X
        float tempHeroPos_y = CHero::GetInstance()->GetHeroPos_y();	//Hero current position Y
        int checkPosition_X = (int)((CHero::GetInstance()->GetMapOffset_x() + tempHeroPos_x) / m_cMap->GetTileSize());	//Hero tile position X
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (tempHeroPos_y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
        int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
        int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

        if(hero_HP != 100) // If not 100 goes in here
        {
            if(hero_HP < 100 && hero_HP > 75) // if Health is in between 75 to 100, set it to 100
            {
                //CHero::GetInstance()->setHero_Health(100);
                hero_HP = 100;
            }
            else if(hero_HP <= 75) // If health is lower or equals to 75, health plus 25.
            {
                //CHero::GetInstance()->setHero_Health(CHero::GetInstance()->Gethero_HP() + 25);
                hero_HP += 25;
            }
        }

        cout << " TOUCH RED" << endl;
         m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 2] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 2] = 0;
    }
        
    if(HeroTileCheck(m_cMap, TILE_SCORE,true,true,true,true,GetHeroPos_x(),GetHeroPos_y(),GetMapOffset_x(),Getjumpspeed()) == true)
    {
        float tempHeroPos_x = CHero::GetInstance()->GetHeroPos_x();	//Hero current position X
        float tempHeroPos_y = CHero::GetInstance()->GetHeroPos_y();	//Hero current position Y
        int checkPosition_X = (int)((CHero::GetInstance()->GetMapOffset_x() + tempHeroPos_x) / m_cMap->GetTileSize());	//Hero tile position X
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (tempHeroPos_y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
        int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
        int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

        hero_SCORE += 100;

        m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 2] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 1] = 0;
        m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 2] = 0;
        
        cout << "TOUCH GREEN -> " << hero_SCORE << endl;
    }

	HeroKnockBack(m_cMap);
	ConstrainHeroX(100, 600, 25, 575, 1.0f,mapWidth,mapHeight,maplevel);
	// Calculate the fine offset
	mapFineOffset_X = mapOffset_X % m_cMap->GetTileSize();
}

CInventory& CHero::getInventory(void)
{
	return this->hero_invent;
}

float& CHero::GetHeroPos_x(void)
{
	return this->HeroPos.x;
}
float& CHero::GetHeroPos_y(void)
{
	return this->HeroPos.y;
}
bool& CHero::Gethero_inMidAir_Up(void)
{
	return this->hero_inMidAir_Up;
}
bool& CHero::Gethero_inMidAir_Down(void)
{
	return this->hero_inMidAir_Down;
}

bool& CHero::Gethero_face_left(void)
{
	return this->hero_face_left;
}
bool& CHero::Gethero_face_right(void)
{
	return this->hero_face_right;
}

int& CHero::Getjumpspeed(void)
{
	return this->jumpspeed;
}
int& CHero::Getjumpcount(void)
{
	return this->jumpcount;
}

int& CHero::Gethero_HP(void)
{
	return this->hero_HP;
}
int& CHero::Gethero_EP(void)
{
	return this->hero_EP;
}

float& CHero::Gethero_MS(void)
{
	return this->hero_MS;
}
float& CHero::Gethero_invulframe(void)
{
	return this->invul_frame;
}

int CHero::GetMapOffset_x(void)
{
	return this->mapOffset_X;
}
int CHero::GetMapOffset_y(void)
{
	return this->mapOffset_Y;
}

void CHero::setMapOffset_x(int x)
{
	this->mapOffset_X = x;
}
void CHero::setMapOffset_y(int y)
{
	this->mapOffset_Y = y;
}

int CHero::GetMapFineOffset_x(void)
{
	return this->mapFineOffset_X;
}
int CHero::GetMapFineOffset_y(void)
{
	return this->mapFineOffset_Y;
}

CHero* CHero::GetInstance()
{
	if (heroInstance == 0)
		heroInstance = new CHero();

	return heroInstance;
}

void CHero::SetHeroPos_x(float input)
{
	this->HeroPos.x = input;
}
void CHero::SetHeroPos_y(float input)
{
	this->HeroPos.y = input;
}

void CHero::setHero_Velocity(float New_Velocity)
{
    hero_velocity = New_Velocity;
}

void CHero::Hero_Acceleration(bool LeftOrRight,const float timeDiff)
{
    f_Current_Acceleration = f_force / hero_mass; // Get Acceleration

    if(LeftOrRight) //Move Left
    {

        if(hero_velocity > -hero_maxspeed)
        {
            hero_velocity -= f_Current_Acceleration * timeDiff;
        }
        else if (hero_velocity <= -hero_maxspeed)
        {
            hero_velocity = -hero_maxspeed;
        }
        
		HeroPos.x = HeroPos.x + (float)(hero_velocity * timeDiff);  // Translate Hero's position according to the velocity
    }
    else           //Move Right
    {
        if(hero_velocity < hero_maxspeed)
        {
            hero_velocity += f_Current_Acceleration * timeDiff;
        }
        else if (hero_velocity >= hero_maxspeed)
        {
            hero_velocity = hero_maxspeed;
        }
        
		HeroPos.x = HeroPos.x + (float)(hero_velocity * timeDiff);
    }
    
}

void CHero::setHero_Deceleration(bool LeftorRight ,float Decelerate)
{
    if(!LeftorRight) // RIGHT
    {
        hero_velocity = Decelerate;
        HeroPos.x += (float)(hero_velocity);
    }
    else //LEFT
    {
        hero_velocity = Decelerate;
        HeroPos.x +=  (float)(hero_velocity);
    }
}

float CHero::getHero_Velocity(void)
{
    return hero_velocity;
}

void CHero::setHero_Health(int newHealth)
{
    hero_HP = newHealth;
}

void CHero::setHero_Invi(bool NewInvi)
{
    this->hero_invisible = NewInvi;
}
bool CHero::getHero_Invi()
{
    return hero_invisible;
}