#include "EnemyBullet.h"

CEnemyBullet::CEnemyBullet(void) :
			f_speed(7.f)
{
}

CEnemyBullet::~CEnemyBullet(void)
{
}

void CEnemyBullet::Update(CMap* m_cMap)
{
	Vector3 dir = getDir();
	dir.Normalize();

	Vector3 newPos = getPos();
	
	newPos += Vector3(dir.x * f_speed,dir.y * f_speed,dir.z * f_speed);
	setPos(newPos);

	if(SingleTileCollisionCheck(m_cMap,true,true,true,true,getPos().x,getPos().y,0))
	{
		setActive(false);
	}
}
