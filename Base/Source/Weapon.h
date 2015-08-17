#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include <vector>
//Sound Engine
#include <irrKlang.h>

using namespace irrklang;

using std::vector;

class CWeapon
{
public:
	CWeapon(void);
	~CWeapon(void);

	void SetData(Mesh* mesh,int guntype,float RECOIL,unsigned totalAmmo,unsigned currentAmmo,unsigned maxAmmo,float RELOAD,Vector3 pos,Vector3 Offset);

	Mesh* getMesh(void);
	float getRecoil(void);
	float getRECOIL_RATE(void);
	unsigned getAmmoCount(void);
	unsigned getCurrentAmmo(void);
	int getIndex(void);
	float getReload(void);
	float getRELOAD_RATE(void);
	unsigned getMaxAmmo(void);
	Vector3 getGunOffset(void);
	Vector3 getGunPos(void);
	float getOffsetX(void);
	float getOffsetY(void);
	float getOffsetZ(void);
	bool getReloading(void);
	bool getAimDown(void);

	void UpdateReload(double dt);
	void Shoot(void);

	void setAmmoCount(unsigned input);
	void setReloading(bool input);
	void setCurrentAmmo(unsigned input);
	void setRecoil(float input);
	void setPos(Vector3 input);
	void setOffSet(Vector3 input);
	void setOffsetX(float input);
	void setOffsetY(float input);
	void setOffsetZ(float input);
	void setAimDown(bool input);

private:
	int Index; //Type of gun
	Mesh* meshPtr;	//mesh of gun
	float RECOIL_RATE;//recoil limiter
	float recoil;	//recoil time
	bool Reloading; //reloading of gun
	bool AimDown; //Aim down sight of gun
	unsigned AmmoCount;	//Available amount of ammo
	unsigned currentAmmo;	//Current amount of ammo in gun
	float reload;	//reload time
	float RELOAD_RATE;	//reload limiter
	unsigned MaxAmmo;	//maximum amount of ammo allowed in gun
	Vector3 gunPosition; //Position of gun
	Vector3 gunOffset;	//Offset of gun from position
};

