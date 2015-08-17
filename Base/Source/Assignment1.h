#ifndef ASSIGNMENT_1_H
#define ASSIGNMENT_1_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>

#include "Minimap.h"
#include "Player.h"
#include "C3DObject.h"
#include "Enemy.h"
//Sound Engine
#include <irrKlang.h>

using namespace irrklang;

using std::vector;

struct FogParam
{
	Color color;		//Fog color
	float start;	//For linear fog
	float end;		//For linear fog
	float density;	//For exponential fog
	int type;		// 0 = linear , 1 = exp, 2 = exp2
	bool enabled;	//Toggle fog
};

class Assignment1 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		//fog
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_OBJECT,
		GEO_TEXT,
		GEO_FLOOR,
		//Fence
		GEO_FENCE,
		//Tree
		GEO_TREE,
		//Skyplane
		GEO_SKYPLANE,
		//Terrain
		GEO_TERRAIN,
		//HUD
		GEO_CROSSHAIR,
		GEO_HEALTHHUD,
		GEO_HEALTHBAR,
		//Guns
		GEO_DEAGLE,
		GEO_M4A4,
		//Enemy,
		GEO_SNOWMAN,
		GEO_MAP_ENEMY,
		//Bullets
		GEO_BULLET,
		//Objects
		GEO_TABLE,
		GEO_AMMOBOX,
		NUM_GEOMETRY,
	};

public:
	Assignment1();
	~Assignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void CameraUpdate(double dt);
	virtual void UpdateCollision(double dt);
	virtual void UpdateCameraStatus(const unsigned char key);
	virtual void UpdateCameraFalseStatus(const unsigned char key);

	void InitHUD(void);
	void InitGL(void);
	void InitCamera(void);
	void InitVariables(void);
	void InitMesh(void);
	void InitGun(void);
	void InitEnemy(void);
	void InitShaders(void);

	void UpdateAmmoBox(double dt);
	void UpdateEnemy(double dt);
	void UpdateGun(double dt);
	void UpdateDebug(double dt);
	void UpdateBullet(double dt);
	void UpdateFire(double dt);
	void UpdateGunAnimation(double dt);
	void UpdateRestart(double dt);

	void RenderAmmoBox(void);
	void RenderEnemy(void);
	void RenderTrees(void);
	void RenderSkyPlane(void);
	void RenderTerrain(void);
	void RenderGun();
	void RenderSkybox();
	void RenderHUD(void);
	void RenderBullet(void);
	void RenderFloor(void);
	void RenderMinimap(void);
	void RenderStaticObjects(void);
	void RenderLight(void);

	// Toggle HUD mode
	void SetHUD(const bool m_bHUDmode);

	void RenderDebug(void);
	void ClearBuffer(void);
	void EnableCamera(void);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderEnemyOnMap2D(Mesh *mesh, bool enableLight, float sizeY = 1.0f,float sizeX=1.0f, float x=0.0f, float y=0.0f,bool rotate = false);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeY = 1.0f,float sizeX=1.0f, float x=0.0f, float y=0.0f,bool rotate = false);

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	float textWidth[256];
	Light lights[2];
	FogParam fog;

	bool bLightEnabled;

	float fps;
	//Tree
	unsigned NumTree;
	vector<float> TreeoffsetZ;

	//Terrain
	vector<unsigned char> m_heightMap;

	//Weapons
	CWeapon* Gun;
	vector<CWeapon*> inventory;
	bool isBack;	//Back at starting position

	float firerate;
	//Bullet
	vector<CBullet> BulletContainer;

	//Player
	CPlayer player;
	unsigned score;

	//==SoundEngine==//
	ISoundEngine* engine;
	ISound* Sound;

	//MiniMap//
	CMinimap* m_cMinimap;

	//Objects
	C3DObject *object;
	vector<C3DObject*> staticObjects;

	//Ammo Crates
	vector<C3DObject*> AmmoBoxes;
	float ammoSpawn;
	float AMMO_RATE;

	//Enemy
	CEnemy* enemy;
	vector<CEnemy*> enemyContainer;
	float enemySpawn;
	float ENEMY_RATE;

};

#endif