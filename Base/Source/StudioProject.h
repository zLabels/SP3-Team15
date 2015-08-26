#ifndef ASSIGNMENT_2_H
#define ASSIGNMENT_2_H

#define ScreenHeight 600
#define ScreenWidth 800

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Vector2.h"

#include "Minimap.h"
#include "C3DObject.h"
#include "Enemy.h"
#include "Map.h"
#include "Hero.h"
#include "SpriteAnimation.h"
#include "Monster.h"
#include "MenuClass.h"
#include "Shuriken.h"
#include "GrapplingHook.h"

//Goodies
#include "GoodiesFactory.h"
#include "Goodies.h"
#include "TreasureChest.h"

//Strategies
#include "Strategy_Kill.h"

//Sound
#include "SoundManager.h"

//Tile Collision
#include "TileCollision.h"

using std::vector;

class StudioProject : public Scene
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
		GEO_OBJECT,
		GEO_TEXT,
		GEO_FLOOR,
		//HUD
		GEO_CROSSHAIR,
		GEO_ICON,
		GEO_HUD_TEMPLATE,
		GEO_HUD_HP,
		GEO_HUD_EP,
		//Background
		GEO_BACKGROUND,
		GEO_LAYER_2,
		GEO_LAYER_3,
		//Tiles,
		GEO_TILESTRUCTURE,
		GEO_TILECAVE,
		GEO_TILE_METALSUPPORT_RIGHT,
		GEO_TILE_METALSUPPORT_LEFT,
		GEO_TILE_METALFLOOR,
		GEO_TILE_METALCORNER,
        GEO_TILE_DOOR,
        GEO_TILE_LASER,
        GEO_TILE_LASER_SWITCH,
        GEO_TILE_HEALTH,
        GEO_TILE_SCORE,
        GEO_TILE_SHURIKEN,
        GEO_TILE_OBJECTIVE,
        GEO_TILE_LASER_SHOOTER_UP,
        GEO_TILE_LASER_SHOOTER_DOWN,
        GEO_TILE_LASER_SHOOTER_LEFT,
        GEO_TILE_LASER_SHOOTER_RIGHT,
		GEO_WINDOW_TOP,
		GEO_WINDOW_LEFT,
		GEO_WINDOW_RIGHT,
		GEO_WINDOW_BOTTOM,
		GEO_REAR_WALL,
		GEO_REAR_GLASS,

		//Hero,
		GEO_TILEHERO,
		GEO_PLAYER_RUN_RIGHT,
		GEO_PLAYER_RUN_LEFT,
		GEO_PLAYER_JUMP_RIGHT,
		GEO_PLAYER_JUMP_LEFT,
		GEO_PLAYER_IDLE_RIGHT,
		GEO_PLAYER_IDLE_LEFT,
		GEO_SABER_IDLE_RIGHT,
		GEO_SABER_IDLE_LEFT,
		GEO_SABER_RUN_RIGHT,
		GEO_SABER_RUN_LEFT,
		GEO_SABER_ATTACK_1_RIGHT,
		GEO_SABER_ATTACK_1_LEFT,
		GEO_SABER_ATTACK_2_RIGHT,
		GEO_SABER_ATTACK_2_LEFT,
		GEO_SABER_SHOCKWAVE_RIGHT,
		GEO_SABER_SHOCKWAVE_LEFT,
		GEO_SABER_JUMP_RIGHT,
		GEO_SABER_JUMP_LEFT,
		//Weapon
		GEO_GRAPPLING_HOOK,
		GEO_SHURIKEN,
		GEO_BULLET,
			//Guard 1
		GEO_GUARD1_WALK_RIGHT,
		GEO_GUARD1_WALK_LEFT,
		GEO_GUARD1_DEATH_RIGHT,
		GEO_GUARD1_ATTACK_RIGHT,
		GEO_GUARD1_ATTACK_LEFT,
			//Ghost
		GEO_GHOST_WALK_RIGHT,
		GEO_GHOST_WALK_LEFT,
		GEO_GHOST_ATTACK_RIGHT,
		GEO_GHOST_ATTACK_LEFT,
		GEO_GHOST_DEATH_RIGHT,
		//MENU
		GEO_MENU_BACKGROUND,
		GEO_MENU_FEEDBACK,
		GEO_PLAY_TEXT,
		GEO_CONTROLS_TEXT,
		GEO_LOAD_TEXT,
		GEO_LEVEL_TEXT,
		//Map transition
		MAP_TRANSITION,
		//Powerup
		GEO_TILE_TREASURECHEST,
		NUM_GEOMETRY,
	};

	enum REAR_TILE_TYPE
	{
		WINDOW_TOP = 1,
		WINDOW_LEFT,
		WINDOW_RIGHT,
		WINDOW_BOTTOM,
		REAR_WALL,
		REAR_GLASS,
		MAX_REAR_TILE,
	};

public:
	StudioProject();
	~StudioProject();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void CameraUpdate(double dt);
	virtual void UpdateCameraStatus(const unsigned char key);
	virtual void UpdateCameraFalseStatus(const unsigned char key);

	void InitHUD(void);	//Initialize Heads up display related
	void InitGL(void);	//Initialize GL
	void InitCamera(void);	//Initialize camera
	void InitVariables(void);	//Initialize variables used
	void InitMesh(void);	//Initialize meshes
	void InitShaders(void);	//Initialize shaders used 
	void InitBackground(void);	//Initialize backgrounds in game
	void InitTiles(void);	//Initialize tiles in game
	void InitHero(void);	//Initialize hero related
	void InitMap(void);	//Initialize map in game
	void InitWeapon(void);	//Initialize weapons in game

	void Reset(bool hasWon);	//Resets the game
	void AttackResponse(CHero::ATTACK_TYPE type);	//Reponse of attacks in game
	void LoadEnemies(unsigned Level);	//Loads enemies on map based on map 
	void LoadMap(int level);	//Load map
	void LoadHero();//Load Hero
    void LoadConsumables(unsigned Level);

	void UpdateDebug(double dt);	//Update debug related information
	void UpdateSprites(double dt);	//Update sprite animations
	void UpdateInput(double dt);	//Updates player inputs

	void UpdateMap(double dt);	//Updates the current map
	void UpdateEnemySprites(double dt);		//Updates enemy sprite animations
	void UpdateWeapon();	//Updates the weapons in the game
    void UpdateTiles(); // Updates the tiles in game

	void RenderWeapon();	//Render player's weapon
	void RenderMenu(int input);	//Render Menu
	void RenderHeroSprites(void);	//Render hero related sprites
	void RenderEnemySprites(CEnemy* enemyInput);	//Render Enemy related sprites
	void RenderHUD(void);	//Render heads up display
	void RenderBackground(void);	//Render background
	void RenderTransition(void);	//Render map transition

	//Enemy related Functions
	void EnemyUpdate(double dt);	//Enemy Update
	
	//Hero related functions
	void HeroUpdate(double dt);	//Update hero

	// Toggle HUD mode
	void SetHUD(const bool m_bHUDmode);

	void RenderDebug(void);	//Render debug related
	void ClearBuffer(void);	//Clearing buffer
	void EnableCamera(void);	//Camera type / enable

	void SaveGame(void);//Save

	void RenderText(Mesh* mesh, std::string text, Color color);	//Method to render text
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);	//Method to render text on screen
	void RenderMesh(Mesh *mesh, bool enableLight);	//Method to render meshes
	void RenderEnemyOnMap2D(Mesh *mesh, bool enableLight, float sizeY = 1.0f,float sizeX=1.0f, float x=0.0f, float y=0.0f,bool rotate = false);	//Method to render enemies on minimap
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeY = 1.0f,float sizeX=1.0f, float x=0.0f, float y=0.0f,bool rotate = false);	//Method to render mesh in 2D
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false,const float rotation = 0.f);	//Render 2D Meshes
	void Render2DSprite(Mesh *mesh, const bool enableLight, const float sizex=1.0f,const float sizey=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false);	//Render 2D Sprites

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;	//Camera

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	float textWidth[256];
	Light lights[2];

	bool bLightEnabled;	//Controls light affecting meshesv 

	float fps;	//Frame rate

	//Goodies and goodies factory
	CGoodiesFactory theGoodiesFactory;
	CGoodies** theArrayOfGoodies;	
	void RenderGoodies(void);	//Render powerups

	//Enemy
	vector<CEnemy*> enemyContainer;	//Vector to contain enemies
	float ENEMY_SIZE;	//Size of enemies
	float ENEMY_OFFSET;	//Offset of enemy for tile

	//Sound
	CSoundManager soundplayer;	//Sound player 

	//Maps
	unsigned m_CurrentLevel;	//Current Level
	CMap* m_cMap;	//Current Map
	CMap* m_cMap_Level1;	//Level 1 map
	CMap* m_cMap_Level2;	//Level 2 map
	CMap* m_cMap_Level3;	//Level 3 map
	CMap* m_cMap_Level4;	//Level 4 map
	CMap* m_cMap_Level5;	//Level 5 map
	CMap* m_cDebug;	//Debug Map

	void RenderTileMap();	//Used to render Tile map
	CSpriteAnimation* MapTransition;	//Used to transite from 1 map to another
	bool Transiting;	//Control for transition of map
	bool Lv1Clear;	//Check if map is cleared
	bool Lv2Clear;
	bool Lv3Clear;
	bool Lv4Clear;
	bool Save;
	bool load;

	//Hero's Info
	float HERO_SIZE;	//Size of hero
	float HERO_OFFSET;	//Offset of hero for tile
	float jump_input_delay;	//Delay input for double jump

	//Menu
	CMenuClass GameMenu;
	
	// Codes for Scrolling
	int tileOffset_x,tileOffset_y;

	//Parallax Scrolling
	CMap* m_cRearMap;
	void RenderRearTileMap();
	int rearWallOffset_x, rearWallOffset_y;
	int rearWallTileOffset_x, rearWallTileOffset_y;
	int rearWallFineOffset_x, rearWallFineOffset_y;
	int choice;

	float f_grappleRotation;

	//Convert into class CLayer
	//Store into array of layers
    float f_deceleration;

    //Tiles || Game related
    int Money_Score;

    bool GetorNot;

    //Vector for the powerups
    vector<CTreasureChest*> Treasure;
    float CollisionRange;
};

#endif