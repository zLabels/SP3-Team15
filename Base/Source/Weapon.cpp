#include "Weapon.h"

CWeapon::CWeapon(void)
{
	Mesh* meshPtr = (NULL);	//mesh of gun
	RECOIL_RATE = 0.f;//recoil limiter
	recoil = 0.f;	//recoil time
	AmmoCount = 0;	//Available amount of ammo
	currentAmmo = 0;	//Current amount of ammo in gun
	reload = 0.f;	//reload time
	RELOAD_RATE = 0.f;	//reload limiter
	MaxAmmo = 0;
	gunOffset.SetZero();
	gunPosition.SetZero();
	Reloading = false;
	AimDown = false;
}

CWeapon::~CWeapon(void)
{
}

void CWeapon::SetData(Mesh* mesh,int guntype,float RECOIL,unsigned totalAmmo,unsigned currentAmmo,unsigned maxAmmo,float RELOAD,Vector3 pos,Vector3 Offset)
{
	this->meshPtr = mesh;
	this->RECOIL_RATE = RECOIL;
	this->AmmoCount = totalAmmo;
	this->RELOAD_RATE = RELOAD;
	this->Index = guntype;
	this->currentAmmo = currentAmmo;
	this->MaxAmmo = maxAmmo;
	this->gunOffset = Offset;
	this->gunPosition = pos;
}

Mesh* CWeapon::getMesh(void)
{
	return meshPtr;
}
float CWeapon::getRecoil(void)
{
	return recoil;
}
float CWeapon::getRECOIL_RATE(void)
{
	return RECOIL_RATE;
}
unsigned CWeapon::getAmmoCount(void)
{
	return AmmoCount;
}
unsigned CWeapon::getCurrentAmmo(void)
{
	return currentAmmo;
}
float CWeapon::getReload(void)
{
	return reload;
}
float CWeapon::getRELOAD_RATE(void)
{
	return RELOAD_RATE;
}

Vector3 CWeapon::getGunOffset(void)
{
	return gunOffset;
}
Vector3 CWeapon::getGunPos(void)
{
	return gunPosition;
}
bool CWeapon::getReloading(void)
{
	return Reloading;
}
bool CWeapon::getAimDown(void)
{
	return AimDown;
}

float CWeapon::getOffsetX(void)
{
	return this->gunOffset.x;
}
float CWeapon::getOffsetY(void)
{
	return this->gunOffset.y;
}
float CWeapon::getOffsetZ(void)
{
	return this->gunOffset.z;
}

int CWeapon::getIndex(void)
{
	return Index;
}
unsigned CWeapon::getMaxAmmo(void)
{
	return MaxAmmo;
}
void CWeapon::UpdateReload(double dt)
{
	if(Reloading == true)
	{
		this->reload += (float)dt;
		if(this->reload > this->RELOAD_RATE)
		{
			unsigned difference = MaxAmmo - currentAmmo;
			if(difference <= AmmoCount)
			{
				AmmoCount -= difference;
				currentAmmo += difference;
			}
			if(difference > AmmoCount)
			{
				currentAmmo += AmmoCount;
				AmmoCount = 0;
			}
			Reloading = false;
			this->reload = 0.f;
		}
	}
}
void CWeapon::Shoot(void)
{
	if(this->currentAmmo > 0)
	{
		this->currentAmmo--;
	}
}

void CWeapon::setAmmoCount(unsigned input)
{
	this->AmmoCount = input;
}
void CWeapon::setPos(Vector3 input)
{
	this->gunPosition = input;
}
void CWeapon::setCurrentAmmo(unsigned input)
{
	this->currentAmmo = input;
}
void CWeapon::setRecoil(float input)
{
	this->recoil = input;
}
void CWeapon::setReloading(bool input)
{
	this->Reloading = input;
}
void CWeapon::setAimDown(bool input)
{
	this->AimDown = input;
}
void CWeapon::setOffSet(Vector3 input)
{
	this->gunOffset = input;
}
void CWeapon::setOffsetX(float input)
{
	this->gunOffset.x = input;
}
void CWeapon::setOffsetY(float input)
{
	this->gunOffset.y = input;
}
void CWeapon::setOffsetZ(float input)
{
	this->gunOffset.z = input;
}