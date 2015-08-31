#include "StudioProject.h"
#include "GL\glew.h"

#include "Application.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include <ostream>
#include <fstream>

StudioProject::StudioProject()
	:m_cMap(NULL),
	tileOffset_x(0),
	tileOffset_y(0),
	rearWallOffset_x(0),
	rearWallOffset_y(0),
	rearWallTileOffset_x(0),
	rearWallTileOffset_y(0),
	rearWallFineOffset_x(0),
	rearWallFineOffset_y(0),
    GetorNot(false),
    CollisionRange(40*40),
    POWERUP_SIZE(25.f)
{

}
StudioProject::~StudioProject()
{
	for(unsigned i = 0; i< 10; ++i)
	{
		delete theArrayOfGoodies[i];
	}

	delete theArrayOfGoodies;

	if(m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void StudioProject::InitGL()
{
	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}
void StudioProject::InitShaders()
{
	m_programID = LoadShaders( "Shader//Fog.vertexshader", "Shader//Fog.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 1500, 500);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 3000.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

}
void StudioProject::InitMesh()
{
	int count = 0;
	std::string data = " ";
	//File reading
	std::ifstream inFile;
	inFile.open("Source//HoboWidth.txt");
	if(inFile.good())
	{
		while(getline(inFile, data))
		{
			textWidth[count] = std::stof(data);
			count++;
		}
		inFile.close();
	}
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//HoboStdFont.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	//GameMenu
	meshList[GEO_MENU_BACKGROUND] = MeshBuilder::Generate2DMesh("GameMenu_background", Color(1, 1, 1), 50, 50, 800,600);
	meshList[GEO_MENU_BACKGROUND]->textureID = LoadTGA("Image//Menu//Menu_Background.tga");

	meshList[GEO_MENU_FEEDBACK] = MeshBuilder::Generate2DMesh("GameMenu_feedback", Color(1, 1, 1), 0, 0, 210, 80);
	meshList[GEO_MENU_FEEDBACK]->textureID = LoadTGA("Image//Menu//SelectionFeedback.tga");

	meshList[GEO_PLAY_TEXT] = MeshBuilder::Generate2DMesh("GameMenuPlayText", Color(1, 1, 1), 0, 0, 100, 30);
	meshList[GEO_PLAY_TEXT]->textureID = LoadTGA("Image//Menu//PlayText.tga");

	meshList[GEO_CONTROLS_TEXT] = MeshBuilder::Generate2DMesh("GameMenuControlsText", Color(1, 1, 1), 0, 0, 200, 30);
	meshList[GEO_CONTROLS_TEXT]->textureID = LoadTGA("Image//Menu//ControlsText.tga");
	
	meshList[GEO_LOAD_TEXT] = MeshBuilder::Generate2DMesh("GameMenuLoadText", Color(1, 1, 1), 0, 0, 100, 30);
	meshList[GEO_LOAD_TEXT]->textureID = LoadTGA("Image//Menu//Menu_Loadtext.tga");
	
	meshList[GEO_LEVEL_TEXT] = MeshBuilder::Generate2DMesh("GameMenuLevelText", Color(1, 1, 1), 0, 0, 250, 30);
	meshList[GEO_LEVEL_TEXT]->textureID = LoadTGA("Image//Menu//Menu_Levelstext.tga");

	meshList[GEO_GRAPPLING_HOOK] = MeshBuilder::Generate2DHook("Grapplinghook", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_GRAPPLING_HOOK]->textureID = LoadTGA("Image//Weapon//Grappling_Hook.tga");
	
	meshList[GEO_GUARD_DETECTION] = MeshBuilder::GenerateQuad("DetectionRange",Color(1,1,1), 225.f);
	meshList[GEO_GUARD_DETECTION]->textureID = LoadTGA("Image//Enemies//Enemy_detection.tga");
}
void StudioProject::InitCamera()
{
	camera.Init(Vector3(-1000, 68, 10), Vector3(-1000, 68, 0), Vector3(0, 1, 0));
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void StudioProject::InitVariables()
{
	//Enemy
	ENEMY_SIZE = 70.f;
	ENEMY_OFFSET = 33.f;

	//Hero
	HERO_SIZE = 70.f;
	jump_input_delay = 0.f;

	//Sound
	soundplayer.Init();

	//Level
	m_CurrentLevel = 1;
	Lv1Clear = false;
	Lv2Clear = false;
	Lv3Clear = false;
	Lv4Clear = false;
	//Map
	Transiting = false;

	//Save
	Save = false;
	load = false;

	choice = 0;

    //Deceleration Physics
    f_deceleration = 20;

    //Init for Tiles || Game related stuff
    Money_Score = 0;
}
void StudioProject::InitHUD()
{
	meshList[GEO_ICON] = MeshBuilder::GenerateQuad("Saber_icon",Color(1, 1, 1), 20.f);
	meshList[GEO_ICON]->textureID = LoadTGA("Image//Saber//Saber_icon.tga");

	meshList[GEO_HUD_TEMPLATE] = MeshBuilder::GenerateQuad("HUD_template",Color(1, 1, 1), 20.f);
	meshList[GEO_HUD_TEMPLATE]->textureID = LoadTGA("Image//HUD//HUD_template.tga");

	meshList[GEO_HUD_HP] = MeshBuilder::GenerateQuad("HUD_healthpoints",Color(1, 1, 1), 20.f);
	meshList[GEO_HUD_HP]->textureID = LoadTGA("Image//HUD//HUD_healthpoints.tga");

	meshList[GEO_HUD_EP] = MeshBuilder::GenerateQuad("HUD_energypoints",Color(1, 1, 1), 20.f);
	meshList[GEO_HUD_EP]->textureID = LoadTGA("Image//HUD//HUD_energypoints.tga");
}
void StudioProject::InitBackground()
{
	//First background
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 900, 600);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//Map//Map_Background.tga");

	//Second layer for parrallax
	meshList[GEO_LAYER_2] = MeshBuilder::Generate2DMesh("Layer2", Color(1, 1, 1), 0, 0, 900, 600);
	meshList[GEO_LAYER_2]->textureID = LoadTGA("Image//Backgroundlayer2.tga");

	//Third layer for parrallax
	meshList[GEO_LAYER_3] = MeshBuilder::Generate2DMesh("Layer3", Color(1, 1, 1), 0, 0, 900, 600);
	meshList[GEO_LAYER_3]->textureID = LoadTGA("Image//Backgroundlayer3.tga");
}
void StudioProject::InitHero()
{
	CHero::GetInstance()->HeroInit(50,500);
	//Load function for the hero 

	//Sprite Animation
	//===================================================================================//
	//				
	//							IDLE													//
	//
	//==================================================================================//
	meshList[GEO_PLAYER_IDLE_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Hero_idle_right",2,2);
	meshList[GEO_PLAYER_IDLE_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_Idle.tga");
	
	CHero::GetInstance()->Hero_idle_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_IDLE_RIGHT]);
	if(CHero::GetInstance()->Hero_idle_right)
	{
		CHero::GetInstance()->Hero_idle_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_idle_right->m_anim->Set(0, 1, 1, 1.f, true,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	meshList[GEO_PLAYER_IDLE_LEFT] = MeshBuilder::GenerateSpriteAnimation("Hero_idle_left",2,2);
	meshList[GEO_PLAYER_IDLE_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_Idle.tga");
	
	CHero::GetInstance()->Hero_idle_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_IDLE_LEFT]);
	if(CHero::GetInstance()->Hero_idle_left)
	{
		CHero::GetInstance()->Hero_idle_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_idle_left->m_anim->Set(2, 3, 1, 1.f, true,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	//===================================================================================//
	//				
	//							RUN													//
	//
	//==================================================================================//

	meshList[GEO_PLAYER_RUN_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Hero_run_right",2,9);
	meshList[GEO_PLAYER_RUN_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_walking_sprite.tga");
	
	CHero::GetInstance()->Hero_run_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_RUN_RIGHT]);
	if(CHero::GetInstance()->Hero_run_right)
	{
		CHero::GetInstance()->Hero_run_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_run_right->m_anim->Set(0, 8, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	meshList[GEO_PLAYER_RUN_LEFT] = MeshBuilder::GenerateSpriteAnimation("Hero_run_left",2,9);
	meshList[GEO_PLAYER_RUN_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_walking_sprite.tga");
	
	CHero::GetInstance()->Hero_run_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_RUN_LEFT]);
	if(CHero::GetInstance()->Hero_run_left)
	{
		CHero::GetInstance()->Hero_run_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_run_left->m_anim->Set(9, 17, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	} 
    

	//===================================================================================//
	//				
	//							ATTACKS													//
	//
	//==================================================================================//
	meshList[GEO_SABER_ATTACK_1_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_attack_1_right",1,7);
	meshList[GEO_SABER_ATTACK_1_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_attack1_right.tga");

	CHero::GetInstance()->Hero_attack_1_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_ATTACK_1_RIGHT]);
	if(CHero::GetInstance()->Hero_attack_1_right)
	{
		CHero::GetInstance()->Hero_attack_1_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_attack_1_right->m_anim->Set(0, 6, 1, 0.7f, false,Vector3(0,0,1),0,0.f,HERO_SIZE + 40.f);
	}

	meshList[GEO_SABER_ATTACK_1_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_attack_1_left",1,7);
	meshList[GEO_SABER_ATTACK_1_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_attack1_left.tga");

	CHero::GetInstance()->Hero_attack_1_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_ATTACK_1_LEFT]);
	if(CHero::GetInstance()->Hero_attack_1_left)
	{
		CHero::GetInstance()->Hero_attack_1_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_attack_1_left->m_anim->Set(0, 6, 1, 0.7f, false,Vector3(0,0,1),0,0.f,HERO_SIZE + 40.f);
	}

	meshList[GEO_SABER_ATTACK_2_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_attack_2_right",1,8);
	meshList[GEO_SABER_ATTACK_2_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_attack2_right.tga");

	CHero::GetInstance()->Hero_attack_2_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_ATTACK_2_RIGHT]);
	if(CHero::GetInstance()->Hero_attack_2_right)
	{
		CHero::GetInstance()->Hero_attack_2_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_attack_2_right->m_anim->Set(0, 7, 1, 0.7f, false,Vector3(0,0,1),0,0.f,HERO_SIZE + 10.f);
	}

	meshList[GEO_SABER_ATTACK_2_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_attack_2_left",1,8);
	meshList[GEO_SABER_ATTACK_2_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_attack2_left.tga");

	CHero::GetInstance()->Hero_attack_2_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_ATTACK_2_LEFT]);
	if(CHero::GetInstance()->Hero_attack_2_left)
	{
		CHero::GetInstance()->Hero_attack_2_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_attack_2_left->m_anim->Set(0, 7, 1, 0.7f, false,Vector3(0,0,1),0,0.f,HERO_SIZE + 10.f);
	}

	meshList[GEO_SABER_SHOCKWAVE_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_shockwave_right",1,6);
	meshList[GEO_SABER_SHOCKWAVE_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_shockwave_right.tga");

	CHero::GetInstance()->Hero_shockwave_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_SHOCKWAVE_RIGHT]);
	if(CHero::GetInstance()->Hero_shockwave_right)
	{
		CHero::GetInstance()->Hero_shockwave_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_shockwave_right->m_anim->Set(0, 5, 0, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	meshList[GEO_SABER_SHOCKWAVE_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_shockwave_left",1,6);
	meshList[GEO_SABER_SHOCKWAVE_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_shockwave_left.tga");

	CHero::GetInstance()->Hero_shockwave_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_SHOCKWAVE_LEFT]);
	if(CHero::GetInstance()->Hero_shockwave_left)
	{
		CHero::GetInstance()->Hero_shockwave_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_shockwave_left->m_anim->Set(0, 5, 0, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	//===================================================================================//
	//				
	//							JUMP													//
	//
	//==================================================================================//
	
	meshList[GEO_PLAYER_JUMP_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Hero_jump_right",1,1);
	meshList[GEO_PLAYER_JUMP_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_Jump_Right.tga");

	CHero::GetInstance()->Hero_jump_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_JUMP_RIGHT]);
	if(CHero::GetInstance()->Hero_jump_right)
	{
		CHero::GetInstance()->Hero_jump_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_jump_right->m_anim->Set(0, 0, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}
	
	meshList[GEO_PLAYER_JUMP_LEFT] = MeshBuilder::GenerateSpriteAnimation("Hero_jump_left",1,1);
	meshList[GEO_PLAYER_JUMP_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_Jump_Left.tga");

	CHero::GetInstance()->Hero_jump_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_PLAYER_JUMP_LEFT]);
	if(CHero::GetInstance()->Hero_jump_left)
	{
		CHero::GetInstance()->Hero_jump_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_jump_left->m_anim->Set(0,0, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}
}
void StudioProject::InitMap()
{
	// Initialise and load the tile map
	m_cMap = new CMap();

	m_cDebug = new CMap();
	m_cDebug->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cDebug->LoadMap( "Image//MapDesigns//Map_Debug.csv" );

	m_cMap_Level1 = new CMap();
	m_cMap_Level1->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cMap_Level1->LoadMap( "Image//MapDesigns//Map_Level1.csv");

	m_cMap_Level2 = new CMap();
	m_cMap_Level2->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cMap_Level2->LoadMap( "Image//MapDesigns//Map_Level1.csv");

	m_cMap_Level3 = new CMap();
	m_cMap_Level3->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cMap_Level3->LoadMap( "Image//MapDesigns//Map_Level3.csv");

	m_cMap_Level4 = new CMap();
	m_cMap_Level4->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cMap_Level4->LoadMap( "Image//MapDesigns//Map_Level4.csv");

	m_cMap_Level5 = new CMap();
	m_cMap_Level5->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 3200 );
	m_cMap_Level5->LoadMap( "Image//MapDesigns//Map_Level1.csv");

	m_cMap = m_cMap_Level1;

	//Init and load rear tile map
	m_cRearMap = new CMap();
	m_cRearMap->Init (600, 800, 24, 32, 600, 3200);
	m_cRearMap->LoadMap("Image//MapDesigns//MapDesign_Rear.csv" );

	//This is the sprite animation seen when transiting from 1 map to another
	meshList[MAP_TRANSITION] = MeshBuilder::GenerateSpriteAnimation("maptransition",1,9);
	meshList[MAP_TRANSITION]->textureArray[0] = LoadTGA("Image//TransitionSprite.tga");
	
	MapTransition = dynamic_cast<CSpriteAnimation*>(meshList[MAP_TRANSITION]);
	if(MapTransition)
	{
		MapTransition->m_anim = new Animation();
		MapTransition->m_anim->Set(0, 8, 0, 1.f, false,Vector3(400,350,1),0,0.f,800);
	}
}
void StudioProject::InitTiles()
{
	meshList[GEO_TILECAVE] = MeshBuilder::Generate2DMesh("GEO_CAVE", Color(1, 1, 1), 0, 0, 70, 80);
	meshList[GEO_TILECAVE]->textureID = LoadTGA("Image//Tiles//CaveTile3.tga");

	/*
		Generate Tile is the function that breaks up the tileset and generate the tile 
		based on the selected col and row, Tile size needs to be 26 
	*/
	//Ground
	meshList[GEO_TILE_METALSUPPORT_LEFT] = MeshBuilder::GenerateTile("Metal_Support_Left",7,14,3,4,26);
	meshList[GEO_TILE_METALSUPPORT_LEFT]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

	meshList[GEO_TILE_METALSUPPORT_RIGHT] = MeshBuilder::GenerateTile("Metal_Support_Right",7,14,3,5,26);
	meshList[GEO_TILE_METALSUPPORT_RIGHT]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

	meshList[GEO_TILE_METALFLOOR] = MeshBuilder::GenerateTile("Metal_floor",7,14,1,2,26);
	meshList[GEO_TILE_METALFLOOR]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

	meshList[GEO_TILE_METALCORNER] = MeshBuilder::GenerateTile("Metal_Corner",7,14,1,4,26);
	meshList[GEO_TILE_METALCORNER]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_DOOR] = MeshBuilder::GenerateTile("Metal_Corner",7,14,0,11,26);
	meshList[GEO_TILE_DOOR]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_LASER_HORIZONTAL] = MeshBuilder::GenerateTile("Laser",7,14,5,10,26);
	meshList[GEO_TILE_LASER_HORIZONTAL]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_LASER_VERTICAL] = MeshBuilder::GenerateTile("Laser",7,14,6,10,26);
	meshList[GEO_TILE_LASER_VERTICAL]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_LASER_SWITCH] = MeshBuilder::GenerateTile("Laser_Switch",7,14,6,0,26);
	meshList[GEO_TILE_LASER_SWITCH]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_HEALTH] = MeshBuilder::GenerateTile("HEALTH",7,14,6,12,26);
	meshList[GEO_TILE_HEALTH]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_SCORE] = MeshBuilder::GenerateTile("Score",7,14,5,13,26);
	meshList[GEO_TILE_SCORE]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_SHURIKEN] = MeshBuilder::GenerateTile("SHURIKEN",7,14,4,8,26);
	meshList[GEO_TILE_SHURIKEN]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

	meshList[GEO_WINDOW_TOP] = MeshBuilder::GenerateTile("GEO_WINDOW",3,7,0,0,26);
	meshList[GEO_WINDOW_TOP]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");

	meshList[GEO_WINDOW_LEFT] = MeshBuilder::GenerateTile("GEO_WINDOW",3,7,2,5,26);
	meshList[GEO_WINDOW_LEFT]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");
	
	meshList[GEO_WINDOW_RIGHT] = MeshBuilder::GenerateTile("GEO_WINDOW",3,7,2,6,26);
	meshList[GEO_WINDOW_RIGHT]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");
	
	meshList[GEO_WINDOW_BOTTOM] = MeshBuilder::GenerateTile("GEO_WINDOW",3,7,1,0,26);
	meshList[GEO_WINDOW_BOTTOM]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");

	meshList[GEO_REAR_WALL] = MeshBuilder::GenerateTile("GEO_REAR_WALL",3,7,2,3,26);
	meshList[GEO_REAR_WALL]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");

	meshList[GEO_REAR_GLASS] = MeshBuilder::GenerateTile("GEO_REAR_GLASS",3,7,2,2,26);
	meshList[GEO_REAR_GLASS]->textureID = LoadTGA("Image//Tiles//Tile_OfficeWindow.tga");

    meshList[GEO_TILE_OBJECTIVE] = MeshBuilder::GenerateTile("Objective",7,14,1,13,26);
	meshList[GEO_TILE_OBJECTIVE]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_LASER_SHOOTER_UP] = MeshBuilder::GenerateTile("Objective",7,14,6,7,26);
	meshList[GEO_TILE_LASER_SHOOTER_UP]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_LASER_SHOOTER_DOWN] = MeshBuilder::GenerateTile("Objective",7,14,5,7,26);
	meshList[GEO_TILE_LASER_SHOOTER_DOWN]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_LASER_SHOOTER_LEFT] = MeshBuilder::GenerateTile("Objective",7,14,5,6,26);
	meshList[GEO_TILE_LASER_SHOOTER_LEFT]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_LASER_SHOOTER_RIGHT] = MeshBuilder::GenerateTile("Objective",7,14,6,6,26);
	meshList[GEO_TILE_LASER_SHOOTER_RIGHT]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

    meshList[GEO_TILE_TREASURECHEST] = MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1,1,1), 0,0,25,25);
	meshList[GEO_TILE_TREASURECHEST]->textureID = LoadTGA("Image//Tiles//tile4_treasurechest.tga");

    meshList[GEO_TILE_STEALTH_BOX] = MeshBuilder::GenerateTile("Objective",1,1,1,1,26);
	meshList[GEO_TILE_STEALTH_BOX]->textureID = LoadTGA("Image//Tiles//Tile_Stealth_Box.tga");
    
    meshList[GEO_TILE_FINISH_CLOSE] = MeshBuilder::GenerateTile("FINISH_CLOSE",7,14,0,7,26);
	meshList[GEO_TILE_FINISH_CLOSE]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");
    
    meshList[GEO_TILE_FINISH_OPEN] = MeshBuilder::GenerateTile("FINISH_OPEN",7,14,0,6,26);
	meshList[GEO_TILE_FINISH_OPEN]->textureID = LoadTGA("Image//Tiles//Tile_Floor.tga");

	theArrayOfGoodies = new CGoodies*[10];
	for(unsigned i = 0; i < 10; ++i)
	{
		theArrayOfGoodies[i] = theGoodiesFactory.Create(CGoodiesFactory::TREASURE_CHEST);
		theArrayOfGoodies[i]->SetPos(150 + i*25, 150);
		theArrayOfGoodies[i]->SetMesh(MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1,1,1), 0,0,25,25));
		theArrayOfGoodies[i]->SetTextureID(LoadTGA("Image//Tiles//tile4_treasurechest.tga"));
	}
    
    //---TREASURE CHEST---
    /*for(int i = 0; i < 1; i++)
    {
        CTreasureChest* Chest = new CTreasureChest();
        Chest->SetActive(true);
        Chest->setPositionX(300);
        Chest->setPositionY(50);
        Treasure.push_back(Chest);
    }*/
}
void StudioProject::InitWeapon()
{
	meshList[GEO_GRAPPLING_HOOK] = MeshBuilder::Generate2DHook("Grapplinghook", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_GRAPPLING_HOOK]->textureID = LoadTGA("Image//Weapon//Grappling_Hook.tga");
	
	meshList[GEO_SHURIKEN] = MeshBuilder::GenerateQuad("Shuriken",Color(1,1,1),25.f);
	meshList[GEO_SHURIKEN]->textureID = LoadTGA("Image//Weapon//Weapon_Shuriken.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateQuad("Bullet",Color(1,1,1),25.f);
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//Weapon//Weapon_Bullet.tga");
}

void StudioProject::Init()
{
	InitGL();
	InitShaders();
	InitCamera();
	InitVariables();

	InitMesh();
	InitHUD();
	InitBackground();
	InitTiles();
	InitMap();
	InitHero();
	InitWeapon();
	LoadEnemies(m_CurrentLevel);
    LoadConsumables(m_CurrentLevel);

}

void StudioProject::Reset(bool hasWon)
{
	/*
	This function resets the game's variables based on the win / lose state.
	If the player has Won the game, the game is resetted differently from when the player has lost the game
	*/
	if(hasWon == false)
	{
		CHero::GetInstance()->HeroInit(50,400);
		CHero::GetInstance()->Gethero_HP() = 100;
		CHero::GetInstance()->Gethero_EP() = 0;
		CHero::GetInstance()->setMapOffset_x(0);
		CHero::GetInstance()->setMapOffset_y(0);

		m_CurrentLevel = 1;

		//Level
		m_CurrentLevel = 1;
		Lv1Clear = false;
		LoadMap(m_CurrentLevel);
		LoadEnemies(m_CurrentLevel);
        LoadConsumables(m_CurrentLevel);
		//Map
		Transiting = false;
		m_cMap = m_cMap_Level1;

		soundplayer.stopSound();
		GameMenu.setReset(false);
	}
	if(hasWon == true)
	{
		CHero::GetInstance()->HeroInit(50,400);
		CHero::GetInstance()->Gethero_HP() = 100;
		CHero::GetInstance()->Gethero_EP() = 0;
		CHero::GetInstance()->setMapOffset_x(0);
		CHero::GetInstance()->setMapOffset_y(0);

		//Level
		m_CurrentLevel = 1;
		Lv1Clear = false;
		LoadEnemies(m_CurrentLevel);

		//Map
		Transiting = false;
		m_cMap = m_cMap_Level1;

		soundplayer.stopSound();
	
		GameMenu.setReset(false);
	}
}

void StudioProject::SaveGame()
{
	if(Save)
	{
		cout<<"SAVE";
		ofstream saveFile/*("Source//TextFiles//Player//Player_Data.txt")*/;
		saveFile.open("Source//TextFiles//Player//Player_Data.txt");
		if(saveFile.is_open())
		{
			saveFile << (CHero::GetInstance()->GetHeroPos_x()) << ",";
			saveFile << (CHero::GetInstance()->GetHeroPos_y()) << ",";

			saveFile << (CHero::GetInstance()->Gethero_HP()) <<",";
			saveFile << (CHero::GetInstance()->Gethero_EP()) <<",";

			saveFile << (CHero::GetInstance()->GetMapOffset_x()) <<",";
			saveFile << (CHero::GetInstance()->GetMapOffset_y()) <<",";

			saveFile << (m_CurrentLevel);

			Save = false;
			load = true;
		}
		saveFile.close();
	}
}
void StudioProject::LoadHero()
{
	string info = ""; //Used to store string from text file
	vector<int> Hero_position;
	if(Save == false)
	{
		if(load == true)
		{
			ifstream herodata("Source//TextFiles//Player//Player_Data.txt");
			if(!herodata.is_open())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "ERROR LOADING", Color(1, 1, 1), 2.f, 15, 42);
			}

			if(herodata.is_open())
			{
				while(herodata.good())
				{
					getline(herodata, info);

					istringstream is(info);
					
					for(string value; getline(is, value, ','); )
					{
						Hero_position.push_back((atoi(value.c_str())));
					}
				}			
				//herodata.seekg(0,std::ios::end);
				//unsigned int size = herodata.tellg();
				//if(size == 0)
				//{
				//	cout<<"NOTHING"<<endl;
				//	RenderTextOnScreen(meshList[GEO_TEXT], "NO SAVED FILE", Color(1, 1, 1), 2.f, 15, 42);
				//}
			}
			load = false;
			herodata.close();
		}
	}
	if(Hero_position.size() > 0)
	{
		unsigned j = 0; // used to control variables passed in
		j+=6;	
		CHero::GetInstance()->HeroInit(Hero_position[j-6],Hero_position[j-5]);
		CHero::GetInstance()->Gethero_HP() = Hero_position[j-4];
		CHero::GetInstance()->Gethero_EP() = Hero_position[j-3];
		CHero::GetInstance()->setMapOffset_x(Hero_position[j-2]);
		CHero::GetInstance()->setMapOffset_y(Hero_position[j-1]);
		LoadMap(Hero_position[j]);
	}

}
void StudioProject::LoadMap(int level)
{
	switch(level)
	{
	case 1:
		{

			m_cMap_Level1->LoadMap( "Image//MapDesigns//Map_Level1.csv");
			m_cMap = m_cMap_Level1;
			LoadEnemies(level);
            LoadConsumables(level);
            GetorNot = false;
            cout << "Loaded" << endl;
		}
		break;
	case 2:
		{
			m_cMap_Level1->LoadMap( "Image//MapDesigns//Map_Level1.csv");
			m_cMap = m_cMap_Level1;
			LoadEnemies(level);
            LoadConsumables(level);
            GetorNot = false;
            cout << "Loaded" << endl;
		}
		break;
	case 3:
		{
			m_cMap_Level3->LoadMap( "Image//MapDesigns//Map_Level3.csv");
			m_cMap = m_cMap_Level3;
			LoadEnemies(level);
            LoadConsumables(level);
            GetorNot = false;
		}
		break;
	case 4:
		{
			CHero::GetInstance()->SetHeroPos_x(100);
			CHero::GetInstance()->SetHeroPos_y(500);

			m_cMap_Level4->LoadMap( "Image//MapDesigns//Map_Level4.csv");
			m_cMap = m_cMap_Level4;
			LoadEnemies(level);
            LoadConsumables(level);
            GetorNot = false;
		}
		break;
	case 5:
		{
			m_cMap_Level5->LoadMap( "Image//MapDesigns//Map_Level1.csv");
			m_cMap = m_cMap_Level5;
			LoadEnemies(level);
            LoadConsumables(level);
            GetorNot = false;
		}
		break;
	}
}
void StudioProject::LoadEnemies(unsigned Level)
{
	/*
	This function reads the enemies data from a text file based on the level
	Example, if 1 is passed into the function, the switch case will open the Level1 text file,
	Add more if required.
	*/
	enemyContainer.clear();	//Clearing the vector of enemies
	string data = "";	//Used to store string from text file
	vector<int> positionData;	//Local vector to store the data from text file to be used later on
	switch(Level)
	{
	case 1:
		{
			if(Lv1Clear == false)
			{
				ifstream datafiles("Source//TextFiles//Enemies//Guard_Level1Data.txt");
				if(datafiles.is_open())
				{
					while(datafiles.good())
					{
						getline(datafiles, data);

						istringstream is(data);
						// If this line is not a comment line, then process it
						if(!(data.find("//") == NULL) && data != "")
						{
							//Stores data into vector
							for(string line; getline(is, line, ','); )
							{
								positionData.push_back((atoi(line.c_str())));
							}
						}
					}
				}
				datafiles.close();
			}
		}
		break;
	case 2:
		{
			ifstream datafiles("Source//TextFiles//Enemies//Guard_Level2Data.txt");
			if(datafiles.is_open())
			{
				while(datafiles.good())
				{
					getline(datafiles, data);

					istringstream is(data);
					// If this line is not a comment line, then process it
					if(!(data.find("//") == NULL) && data != "")
					{
						//Stores data into vector
						for(string line; getline(is, line, ','); )
						{
							positionData.push_back((atoi(line.c_str())));
						}
					}
				}
			}
			datafiles.close();
		}
		break;
	case 3:
		{
			ifstream datafiles("Source//TextFiles//Enemies//Guard_Level3Data.txt");
			if(datafiles.is_open())
			{
				while(datafiles.good())
				{
					getline(datafiles, data);

					istringstream is(data);
					// If this line is not a comment line, then process it
					if(!(data.find("//") == NULL) && data != "")
					{
						//Stores data into vector
						for(string line; getline(is, line, ','); )
						{
							positionData.push_back((atoi(line.c_str())));
						}
					}
				}
			}
			datafiles.close();
		}
		break;
	case 4:
		{
			ifstream datafiles("Source//TextFiles//Enemies//Guard_Level4Data.txt");
			if(datafiles.is_open())
			{
				while(datafiles.good())
				{
					getline(datafiles, data);

					istringstream is(data);
					// If this line is not a comment line, then process it
					if(!(data.find("//") == NULL) && data != "")
					{
						//Stores data into vector
						for(string line; getline(is, line, ','); )
						{
							positionData.push_back((atoi(line.c_str())));
						}
					}
				}
			}
			datafiles.close();
		}
		break;
	case 5:
		{
			ifstream datafiles("Source//TextFiles//Enemies//Guard_Level5Data.txt");
			if(datafiles.is_open())
			{
				while(datafiles.good())
				{
					getline(datafiles, data);

					istringstream is(data);
					// If this line is not a comment line, then process it
					if(!(data.find("//") == NULL) && data != "")
					{
						//Stores data into vector
						for(string line; getline(is, line, ','); )
						{
							positionData.push_back((atoi(line.c_str())));
						}
					}
				}
			}
			datafiles.close();
		}
		break;
	}

	/*=================================================================================					
	SKELETON MONSTER										
	==================================================================================*/

	/*
	This part is to initialize the monsters based on the data stored into the vector.
	So only if the text file has something inside that is stored into the vector,
	this part will be triggered
	*/
	if(positionData.size() > 0)
	{
		int j = -1; // used to control variables passed in
		for(unsigned i = 0; i < (positionData.size() / 6); ++i)
		{	
			j += 6;
			CEnemy* enemy = new CEnemy();
			/*
			Guard 1
			*/
			if(positionData[j-1] == 1)
			{
				enemy->Init(positionData[j-5],positionData[j-4],positionData[j-3],positionData[j-2],true,CEnemy::GUARD1,positionData[j]);
				enemy->ChangeStrategy(new CStrategy_Kill());

				//Right walk
				meshList[GEO_GUARD1_WALK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Guard1_walk_right",3,5);
				meshList[GEO_GUARD1_WALK_RIGHT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Guard_AlertRight.tga");
				enemy->WalkAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_WALK_RIGHT]);
				if(enemy->WalkAnimation_Right)
				{
					enemy->WalkAnimation_Right->m_anim = new Animation();
					enemy->WalkAnimation_Right->m_anim->Set(0, 14, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Left walk
				meshList[GEO_GUARD1_WALK_LEFT] = MeshBuilder::GenerateSpriteAnimation("Guard1_walk_Left",3,5);
				meshList[GEO_GUARD1_WALK_LEFT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Guard_AlertLeft.tga");
				enemy->WalkAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_WALK_LEFT]);
				if(enemy->WalkAnimation_Left)
				{
					enemy->WalkAnimation_Left->m_anim = new Animation();
					enemy->WalkAnimation_Left->m_anim->Set(0, 14, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Death animation
				meshList[GEO_GUARD1_DEATH_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Guard1_dying_Right",3,10);
				meshList[GEO_GUARD1_DEATH_RIGHT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Death//Guard_DyingRight.tga");
				enemy->DeathAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_DEATH_RIGHT]);
				if(enemy->DeathAnimation_Right)
				{
					enemy->DeathAnimation_Right->m_anim = new Animation();
					enemy->DeathAnimation_Right->m_anim->Set(0, 6, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}
				
				meshList[GEO_GUARD1_DEATH_LEFT] = MeshBuilder::GenerateSpriteAnimation("Guard1_dying_Left",3,10);
				meshList[GEO_GUARD1_DEATH_LEFT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Death//Guard_DyingLeft.tga");
				enemy->DeathAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_DEATH_LEFT]);
				if(enemy->DeathAnimation_Left)
				{
					enemy->DeathAnimation_Left->m_anim = new Animation();
					enemy->DeathAnimation_Left->m_anim->Set(0, 6, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}
				
				//Attacking animation
				meshList[GEO_GUARD1_ATTACK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Guard1_attack_right",1,2);
				meshList[GEO_GUARD1_ATTACK_RIGHT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Guard_AttackR.tga");
				enemy->AttackAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_ATTACK_RIGHT]);
				if(enemy->AttackAnimation_Right)
				{
					enemy->AttackAnimation_Right->m_anim = new Animation();
					enemy->AttackAnimation_Right->m_anim->Set(0, 1, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				meshList[GEO_GUARD1_ATTACK_LEFT] = MeshBuilder::GenerateSpriteAnimation("Guard1_attack_left",1,2);
				meshList[GEO_GUARD1_ATTACK_LEFT]->textureArray[0] = LoadTGA("Image//Enemies//Normal//Guard_AttackL.tga");
				enemy->AttackAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GUARD1_ATTACK_LEFT]);
				if(enemy->AttackAnimation_Left)
				{
					enemy->AttackAnimation_Left->m_anim = new Animation();
					enemy->AttackAnimation_Left->m_anim->Set(0, 1, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				enemyContainer.push_back(enemy);
			}
			/*
			Guard 2
			*/
			else if(positionData[j-1] == 2)
			{
				enemy->Init(positionData[j-5],positionData[j-4],positionData[j-3],positionData[j-2],true,CEnemy::GHOST,positionData[j]);
				enemy->ChangeStrategy(new CStrategy_Kill());

				//Right walk
				meshList[GEO_GHOST_WALK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("ghost_Walk_right",1,9);
				meshList[GEO_GHOST_WALK_RIGHT]->textureArray[0] = LoadTGA("Image//GhostMonster//GhostWalk_Right.tga");
				enemy->WalkAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GHOST_WALK_RIGHT]);
				if(enemy->WalkAnimation_Right)
				{
					enemy->WalkAnimation_Right->m_anim = new Animation();
					enemy->WalkAnimation_Right->m_anim->Set(1, 8, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Left Walk
				meshList[GEO_GHOST_WALK_LEFT] = MeshBuilder::GenerateSpriteAnimation("ghost_walk_left",1,9);
				meshList[GEO_GHOST_WALK_LEFT]->textureArray[0] = LoadTGA("Image//GhostMonster//GhostWalk_Left.tga");
				enemy->WalkAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GHOST_WALK_LEFT]);
				if(enemy->WalkAnimation_Left)
				{
					enemy->WalkAnimation_Left->m_anim = new Animation();
					enemy->WalkAnimation_Left->m_anim->Set(1, 8, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Death animation
				meshList[GEO_GHOST_DEATH_RIGHT] = MeshBuilder::GenerateSpriteAnimation("ghost_death_right",1,7);
				meshList[GEO_GHOST_DEATH_RIGHT]->textureArray[0] = LoadTGA("Image//GhostMonster//GhostDeath_Right.tga");
				enemy->DeathAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GHOST_DEATH_RIGHT]);
				if(enemy->DeathAnimation_Right)
				{
					enemy->DeathAnimation_Right->m_anim = new Animation();
					enemy->DeathAnimation_Right->m_anim->Set(0, 6, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Attacking animation
				meshList[GEO_GHOST_ATTACK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("ghost_attack_right",1,4);
				meshList[GEO_GHOST_ATTACK_RIGHT]->textureArray[0] = LoadTGA("Image//GhostMonster//GhostAttack_Right.tga");
				enemy->AttackAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GHOST_ATTACK_RIGHT]);
				if(enemy->AttackAnimation_Right)
				{
					enemy->AttackAnimation_Right->m_anim = new Animation();
					enemy->AttackAnimation_Right->m_anim->Set(0, 3, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,100.f);
				}

				meshList[GEO_GHOST_ATTACK_LEFT] = MeshBuilder::GenerateSpriteAnimation("ghost_attack_left",1,4);
				meshList[GEO_GHOST_ATTACK_LEFT]->textureArray[0] = LoadTGA("Image//GhostMonster//GhostAttack_Left.tga");
				enemy->AttackAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_GHOST_ATTACK_LEFT]);
				if(enemy->AttackAnimation_Left)
				{
					enemy->AttackAnimation_Left->m_anim = new Animation();
					enemy->AttackAnimation_Left->m_anim->Set(0, 3, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,100.f);
				}

				enemyContainer.push_back(enemy);
			}
		}
	}
}
void StudioProject::LoadConsumables(unsigned level)
{
    /*
	This function reads the enemies data from a text file based on the level
	Example, if 1 is passed into the function, the switch case will open the Level1 text file,
	Add more if required.
	*/
	ConsumablePosition.clear();
	Treasure.clear();	//Clearing the vector of treasures
	string data = "";	//Used to store string from text file
	//vector<int> ConsumablePosition;	//Local vector to store the data from text file to be used later on

    switch(level)
    {
    case 1:
        {
            ifstream datafiles("Source//TextFiles//Powerups//Powerups_Level1Data.txt");
            if(datafiles.is_open())
            {
                while(datafiles.good())
                {
                    getline(datafiles, data);

                    istringstream is(data);
                    // If this line is not a comment line, then process it
                    if(!(data.find("//") == NULL) && data != "")
                    {
                        //Stores data into vector

                        for(string line; getline(is, line, ','); )
                        {
                            ConsumablePosition.push_back((atoi(line.c_str())));
                        }
                    }
                }
            }
            else
            {
                cout << "Error on opening" << endl;
            }
            datafiles.close();
        }
        break;

    case 2:
        {
            ifstream datafiles("Source//TextFiles//Powerups//Powerups_Level2Data.txt");
            if(datafiles.is_open())
            {
                while(datafiles.good())
                {
                    getline(datafiles, data);

                    istringstream is(data);
                    // If this line is not a comment line, then process it
                    if(!(data.find("//") == NULL) && data != "")
                    {
                        //Stores data into vector
                        for(string line; getline(is, line, ','); )
                        {
                            ConsumablePosition.push_back((atoi(line.c_str())));
                        }
                    }
                }
            }
            else
            {
                cout << "Error on opening" << endl;
            }
            datafiles.close();
        }
        break;
    case 3:
        {
            ifstream datafiles("Source//TextFiles//Powerups//Powerups_Level3Data.txt");
            if(datafiles.is_open())
            {
                while(datafiles.good())
                {
                    getline(datafiles, data);

                    istringstream is(data);
                    // If this line is not a comment line, then process it
                    if(!(data.find("//") == NULL) && data != "")
                    {
                        //Stores data into vector
                        for(string line; getline(is, line, ','); )
                        {
                            ConsumablePosition.push_back((atoi(line.c_str())));
                        }
                    }
                }
            }
            else
            {
                cout << "Error on opening" << endl;
            }
            datafiles.close();
        }
        break;
    case 4:
        {

            ifstream datafiles("Source//TextFiles//Powerups//Powerups_Level4Data.txt");
            if(datafiles.is_open())
            {
                while(datafiles.good())
                {
                    getline(datafiles, data);

                    istringstream is(data);
                    // If this line is not a comment line, then process it
                    if(!(data.find("//") == NULL) && data != "")
                    {
                        //Stores data into vector
                        for(string line; getline(is, line, ','); )
                        {
                            ConsumablePosition.push_back((atoi(line.c_str())));
                        }
                    }
                }
            }
            else
            {
                cout << "Error on opening" << endl;
            }
            datafiles.close();

            break;
        }
    case 5:
        {
            ifstream datafiles("Source//TextFiles//Powerups//Powerups_Level5Data.txt");
            if(datafiles.is_open())
            {
                while(datafiles.good())
                {
                    getline(datafiles, data);

                    istringstream is(data);
                    // If this line is not a comment line, then process it
                    if(!(data.find("//") == NULL) && data != "")
                    {
                        //Stores data into vector
                        for(string line; getline(is, line, ','); )
                        {
                            ConsumablePosition.push_back((atoi(line.c_str())));
                        }
                    }
                }
            }
            else
            {
                cout << "Error on opening" << endl;
            }
            datafiles.close();
        }
        break;
    }
        
    if(ConsumablePosition.size() > 0)
    {
        int j = -1;
        for(unsigned i = 0; i < (ConsumablePosition.size() / 3); ++i)
        {
            j += 3;
            CTreasureChest * Chest = new CTreasureChest();
            Chest->ChestInit(ConsumablePosition[j-2],ConsumablePosition[j-1],ConsumablePosition[j],true);

            if(ConsumablePosition[j] == 1)
            {
                meshList[GEO_POWERUP_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("Health",3,10);
                meshList[GEO_POWERUP_ANIMATION]->textureArray[0] = LoadTGA("Image//PowerUp//PowerUp_Sprite.tga");
                Chest->Powerup_Animation = dynamic_cast<CSpriteAnimation*>(meshList[GEO_POWERUP_ANIMATION]);
                if(Chest->Powerup_Animation)
                {
                    Chest->Powerup_Animation->m_anim = new Animation();
                    Chest->Powerup_Animation->m_anim->Set(10, 17, 1, 1.f, true,Vector3((float)ConsumablePosition[j-2],(float)ConsumablePosition[j-1],1),0,0.f,25.f);
                }
            }
            if(ConsumablePosition[j] == 2)
            {
                meshList[GEO_POWERUP_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("Score",3,10);
                meshList[GEO_POWERUP_ANIMATION]->textureArray[0] = LoadTGA("Image//PowerUp//PowerUp_Sprite.tga");
                Chest->Powerup_Animation = dynamic_cast<CSpriteAnimation*>(meshList[GEO_POWERUP_ANIMATION]);
                if(Chest->Powerup_Animation)
                {
                    Chest->Powerup_Animation->m_anim = new Animation();
                    Chest->Powerup_Animation->m_anim->Set(0, 9, 1, 1.f, true,Vector3((float)ConsumablePosition[j-2],(float)ConsumablePosition[j-1],1),0,0.f,25.f);
                }
            }
            if(ConsumablePosition[j] == 3)
            {
                meshList[GEO_POWERUP_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("Energy",3,10);
                meshList[GEO_POWERUP_ANIMATION]->textureArray[0] = LoadTGA("Image//PowerUp//PowerUp_Sprite.tga");
                Chest->Powerup_Animation = dynamic_cast<CSpriteAnimation*>(meshList[GEO_POWERUP_ANIMATION]);
                if(Chest->Powerup_Animation)
                {
                    Chest->Powerup_Animation->m_anim = new Animation();
                    Chest->Powerup_Animation->m_anim->Set(20, 29, 1, 1.f, true,Vector3((float)ConsumablePosition[j-2],(float)ConsumablePosition[j-1],1),0,0.f,25.f);
                }
            }
            if(ConsumablePosition[j] == 4)
            {
                meshList[GEO_POWERUP_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("Shuriken",1,5);
                meshList[GEO_POWERUP_ANIMATION]->textureArray[0] = LoadTGA("Image//PowerUp//Shuriken.tga");
                Chest->Powerup_Animation = dynamic_cast<CSpriteAnimation*>(meshList[GEO_POWERUP_ANIMATION]);
                if(Chest->Powerup_Animation)
                {
                    Chest->Powerup_Animation->m_anim = new Animation();
                    Chest->Powerup_Animation->m_anim->Set(0,4,1,1.f,true,Vector3((float)ConsumablePosition[j-2],(float)ConsumablePosition[j-1],1),0,0.f,25.f);
                }
            }

            Treasure.push_back(Chest);
        }
    }
}

void StudioProject::UpdateDebug(double dt)
{
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	if(Application::IsKeyPressed('N'))
	{
		camera.NoClip = false;
	}
	else if(Application::IsKeyPressed('M'))
	{
		camera.NoClip = true;
	}
}
void StudioProject::CameraUpdate(double dt)
{			
	camera.Update(dt);
}
void StudioProject::UpdateCameraStatus(const unsigned char key)
{
	camera.UpdateStatus(key);
}
void StudioProject::UpdateCameraFalseStatus(const unsigned char key)
{
	camera.UpdateFalseStatus(key);
}

void StudioProject::EnemyUpdate(double dt)
{
	float tempHeroPos_x = CHero::GetInstance()->GetHeroPos_x();	//Hero current position X
	float tempHeroPos_y = CHero::GetInstance()->GetHeroPos_y();	//Hero current position Y
	
	//Updates the enemies
	for(std::vector<CEnemy *>::iterator it = enemyContainer.begin(); it != enemyContainer.end(); ++it)
	{
		CEnemy *Enemy = (CEnemy *)*it;
		
		//Updating enemy if active
		if(Enemy->getActive())
		{
			Enemy->SetDestination((int)tempHeroPos_x + CHero::GetInstance()->GetMapOffset_x(),(int)tempHeroPos_y +CHero::GetInstance()->GetMapOffset_y());
			Enemy->Update(m_cMap,ScreenWidth,ScreenHeight,m_CurrentLevel,CHero::GetInstance()->getHero_Invi());

			/*int enemy_x = Enemy->GetPos_x() + CHero::GetInstance()->GetMapOffset_x();
			int enemy_y = Enemy->GetPos_y();
			Enemy->SetPos_x(enemy_x);*/
		}
			//Plays sound if enemy is dead
		else if(Enemy->DeathAnimation_Right->m_anim->animActive == true)
		{
			if(Enemy->enemyType == CEnemy::GUARD1)
			{
				soundplayer.playSounds(soundplayer.SKELE_DEATH);
			}
			else if(Enemy->enemyType == CEnemy::GHOST)
			{
				soundplayer.playSounds(soundplayer.GHOST_DEATH);
			}
		}
		else if(Enemy->DeathAnimation_Left->m_anim->animActive == true)
		{
			if(Enemy->enemyType == CEnemy::GUARD1)
			{
				soundplayer.playSounds(soundplayer.SKELE_DEATH);
			}
			else if(Enemy->enemyType == CEnemy::GHOST)
			{
				soundplayer.playSounds(soundplayer.GHOST_DEATH);
			}
		}
		//Hero getting attacked by enemy
		if(Enemy->getHealth() > 0)	//Only if enemy is alive
		{
			float hero_x = CHero::GetInstance()->GetHeroPos_x();
			hero_x += CHero::GetInstance()->GetMapOffset_x();
			float hero_y = CHero::GetInstance()->GetHeroPos_y();

			if(Enemy->getStrategy()->getState() == 2)
			{
				Enemy->EnemyAttack(meshList[GEO_BULLET],hero_x,hero_y);
				
			}
			if(Enemy->getAttackCD()== 2.f)
			{
				soundplayer.playSounds(soundplayer.RIFLE);
			}
			
		}
		//Enemy Bullet updating
		for(unsigned i = 0;i < Enemy->getEnemyBullet().size(); ++i)
		{
			if( Enemy->getEnemyBullet().at(i)->getActive() == true)
			{
				Enemy->getEnemyBullet().at(i)->Update(m_cMap);
				if( ((CHero::GetInstance()->GetHeroPos_x() + 12.5f) - (Enemy->getEnemyBullet().at(i)->getPos().x - CHero::GetInstance()->GetMapOffset_x())) * ((CHero::GetInstance()->GetHeroPos_x() + 12.5f) - (Enemy->getEnemyBullet().at(i)->getPos().x - CHero::GetInstance()->GetMapOffset_x())) + 
					((CHero::GetInstance()->GetHeroPos_y() + 25.f) - Enemy->getEnemyBullet().at(i)->getPos().y) * ((CHero::GetInstance()->GetHeroPos_y() + 25.f) - Enemy->getEnemyBullet().at(i)->getPos().y) < 1600 )
				{
					CHero::GetInstance()->HeroDamaged(Enemy->getEnemyBullet().at(i)->getDamage());
					Enemy->getEnemyBullet().at(i)->setActive(false);
				}
					//soundplayer.playSounds(soundplayer.RIFLE);
				
			}
		}
		
		//Enemy attacking cool down 
		if(Enemy->getAttackCD() != 0)
		{
			Enemy->getAttackCD() -= (float)dt;
			if(Enemy->getAttackCD() < 0)
			{
				Enemy->getAttackCD() = 0;

			}
		}

		//Enemy getting attacked
		for(unsigned i = 0; i < CHero::GetInstance()->getInventory().getShurikenList().size(); ++i)
		{
			if(CHero::GetInstance()->getInventory().getShurikenList().at(i)->getActive() && Enemy->getActive())
			{
				if( ((Enemy->GetPos_x() + 12.5f) - (CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().x + CHero::GetInstance()->GetMapOffset_x())) * 
					((Enemy->GetPos_x() + 12.5f) - (CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().x + CHero::GetInstance()->GetMapOffset_x())) + 
					((Enemy->GetPos_y() + 25.f) - CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().y) * 
					((Enemy->GetPos_y() + 25.f) - CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().y) < 1600)
				{
					Enemy->EnemyDamaged(100,m_cMap);
					CHero::GetInstance()->getInventory().getShurikenList().at(i)->setActive(false);
				}			
			}
		}
	}
}
void StudioProject::HeroUpdate(double dt)
{
	CHero::GetInstance()->Update(m_cMap,m_cMap->getNumOfTiles_MapWidth() * m_cMap->GetTileSize() - ScreenWidth,ScreenHeight,m_CurrentLevel);	//Update hero

	CHero::GetInstance()->HeroRegen(dt);

	if(CHero::GetInstance()->Gethero_invulframe() != 0)
	{
		CHero::GetInstance()->Gethero_invulframe() -= (float)dt;
		if(CHero::GetInstance()->Gethero_invulframe() < 0)
		{
			CHero::GetInstance()->Gethero_invulframe() = 0;
		}
	}

	if(CHero::GetInstance()->Gethero_HP() <= 0)
	{
		GameMenu.setLostState(true);
	}

	tileOffset_x = (int) (CHero::GetInstance()->GetMapOffset_x() / m_cMap->GetTileSize());
	if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
		tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();
}
void StudioProject::UpdateSprites(double dt)
{
	static bool invisLoaded = false;
    if( ( CHero::GetInstance()->getHero_Invi() == true && invisLoaded == false ) )
    {
        //----------IDLE----------//
	    meshList[GEO_PLAYER_IDLE_LEFT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_Idle_Invisible.tga");           //Left
	    meshList[GEO_PLAYER_IDLE_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_Idle_Invisible.tga");          //Right
        //------------------------//

        //----------RUN----------//
        meshList[GEO_PLAYER_RUN_LEFT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_walking_invisible_sprite.tga");  //Left
        meshList[GEO_PLAYER_RUN_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_walking_invisible_sprite.tga"); //Right
        //-----------------------//

        //----------JUMP----------//
        meshList[GEO_PLAYER_JUMP_LEFT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_Jump_Left_Invisible.tga");      //Left
        meshList[GEO_PLAYER_JUMP_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Invisible//Player_Jump_Right_Invisible.tga");    //Right
        //-----------------------//
		invisLoaded = true;
    }
    else if( (CHero::GetInstance()->getHero_Invi() == false && invisLoaded == true) )
    {
        //----------IDLE----------//
        meshList[GEO_PLAYER_IDLE_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_Idle.tga");               //Left
        meshList[GEO_PLAYER_IDLE_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_Idle.tga");              //Right
        //------------------------//

        //----------RUN----------//
        meshList[GEO_PLAYER_RUN_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_walking_sprite.tga");      //Left
        meshList[GEO_PLAYER_RUN_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_walking_sprite.tga");     //Right
        //-----------------------//

        //----------JUMP----------//
        meshList[GEO_PLAYER_JUMP_LEFT]->textureArray[0] = LoadTGA("Image//Player//Player_Jump_Left.tga");          //Left
        meshList[GEO_PLAYER_JUMP_RIGHT]->textureArray[0] = LoadTGA("Image//Player//Player_Jump_Right.tga");        //Right
        //-----------------------//
		invisLoaded = false;
    }
	//==================================//
	//	Jump 							//
	//==================================//
	CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position

	CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position

	/*
		Check if hero is jumping or falling
		&&
		Check if hero is using any forms of attacks
	*/
	if((CHero::GetInstance()->Gethero_inMidAir_Up() == true || CHero::GetInstance()->Gethero_inMidAir_Down() == true) && 
		(CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == false && 
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == false && 
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false))
	{
		if(CHero::GetInstance()->Gethero_face_right() == true)	//Check if hero is facing right
		{
			CHero::GetInstance()->Hero_jump_right->m_anim->animActive = true;
			CHero::GetInstance()->Hero_jump_left->m_anim->animActive = false;
		}
		if(CHero::GetInstance()->Gethero_face_left() == true)	//Check if hero is facing left
		{
			CHero::GetInstance()->Hero_jump_left->m_anim->animActive = true;
			CHero::GetInstance()->Hero_jump_right->m_anim->animActive = false;
		}
	}
	else
	{
		CHero::GetInstance()->Hero_jump_right->m_anim->animActive = false;	
		CHero::GetInstance()->Hero_jump_left->m_anim->animActive = false;
	}

	if(CHero::GetInstance()->Hero_jump_right)
	{
		CHero::GetInstance()->Hero_jump_right->Update(dt);
	}
	if(CHero::GetInstance()->Hero_jump_left)
	{
		CHero::GetInstance()->Hero_jump_right->Update(dt);
	}

	//===================================//
	//	Run Right						//
	//=================================//
	CHero::GetInstance()->Hero_run_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_run_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	/*
		If hero is jumping, running animation is set to false
	*/
	if(CHero::GetInstance()->Gethero_inMidAir_Down() == true || CHero::GetInstance()->Gethero_inMidAir_Up() == true)
	{
		CHero::GetInstance()->Hero_run_right->m_anim->animActive = false;
	}
	if(CHero::GetInstance()->Hero_run_right)
	{
		CHero::GetInstance()->Hero_run_right->Update(dt);
	}

	//===================================//
	//	Run Left						//
	//=================================//
	CHero::GetInstance()->Hero_run_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_run_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	/*
		If hero is jumping, running animation is set to false
	*/
	if(CHero::GetInstance()->Gethero_inMidAir_Down() == true || CHero::GetInstance()->Gethero_inMidAir_Up() == true)
	{
		CHero::GetInstance()->Hero_run_left->m_anim->animActive = false;
	}
	if(CHero::GetInstance()->Hero_run_left)
	{
		CHero::GetInstance()->Hero_run_left->Update(dt);
	}

	//===================================//
	//	IDLE							//
	//=================================//
	CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position	
	CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	/*
		Checks if hero is doing any other actions at all
	*/
	if(CHero::GetInstance()->Hero_run_right->m_anim->animActive == true || CHero::GetInstance()->Hero_run_left->m_anim->animActive == true ||
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == true || CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == true ||
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == true || CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == true ||
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == true || CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == true ||
		CHero::GetInstance()->Hero_jump_right->m_anim->animActive == true || CHero::GetInstance()->Hero_jump_left->m_anim->animActive == true)
	{
		CHero::GetInstance()->Hero_idle_right->m_anim->animActive = false;
		CHero::GetInstance()->Hero_idle_left->m_anim->animActive = false;
	}
	//else idle animation begins
	else
	{
		if(CHero::GetInstance()->Gethero_face_right() == true)
		{
			CHero::GetInstance()->Hero_idle_right->m_anim->animActive = true;
			CHero::GetInstance()->Hero_idle_left->m_anim->animActive = false;
		}
		else if(CHero::GetInstance()->Gethero_face_left() == true)
		{
			CHero::GetInstance()->Hero_idle_left->m_anim->animActive = true;
			CHero::GetInstance()->Hero_idle_right->m_anim->animActive = false;
		}
	}
	if(CHero::GetInstance()->Hero_idle_left)
	{
		CHero::GetInstance()->Hero_idle_left->Update(dt);
	}
	
	if(CHero::GetInstance()->Hero_idle_right)
	{
		CHero::GetInstance()->Hero_idle_right->Update(dt);
	}
	
	/*CSpriteAnimation *s_run = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_RUN_RIGHT]);
	if(s_run)
	{
		s_run->Update(dt);
	}*/
}
void StudioProject::UpdateEnemySprites(double dt)
{
	for(unsigned i = 0; i < enemyContainer.size(); ++i)
	{
		if(enemyContainer[i]->WalkAnimation_Right)
		{
			enemyContainer[i]->WalkAnimation_Right->Update(dt);
		}
		if(enemyContainer[i]->WalkAnimation_Left)
		{
			enemyContainer[i]->WalkAnimation_Left->Update(dt);
		}
		if(enemyContainer[i]->DeathAnimation_Right)
		{
			enemyContainer[i]->DeathAnimation_Right->Update(dt);
		}
		if(enemyContainer[i]->DeathAnimation_Left)
		{
			enemyContainer[i]->DeathAnimation_Left->Update(dt);
		}
		if(enemyContainer[i]->AttackAnimation_Right)
		{
			enemyContainer[i]->AttackAnimation_Right->Update(dt);
		}
		if(enemyContainer[i]->AttackAnimation_Left)
		{
			enemyContainer[i]->AttackAnimation_Left->Update(dt);
		}
	}
}
void StudioProject::UpdateWeapon()
{
	if(CHero::GetInstance()->getInventory().getShurikenList().size() > 0)
	{
		for(unsigned i = 0;i < CHero::GetInstance()->getInventory().getShurikenList().size(); ++i)
		{
			if(CHero::GetInstance()->getInventory().getShurikenList().at(i)->getActive() == true)
			{
				CHero::GetInstance()->getInventory().getShurikenList().at(i)->Update(m_cMap,CHero::GetInstance()->GetMapOffset_x(),CHero::GetInstance()->GetMapOffset_y());
			}
		}
	}

	if(CHero::GetInstance()->getInventory().getGrapple().getActive())
	{
		CHero::GetInstance()->getInventory().getGrapple().Update(m_cMap,CHero::GetInstance()->GetMapOffset_x(),CHero::GetInstance()->GetMapOffset_y());
	}
}

void StudioProject::UpdatePowerUp(double dt)
{
    for(std::vector<CTreasureChest *>::iterator it = Treasure.begin(); it != Treasure.end(); ++it)
    {
        float hero_x = CHero::GetInstance()->GetHeroPos_x();
        hero_x += 25;
        hero_x += CHero::GetInstance()->GetMapOffset_x(); // Translate the power up according to map offset
		hero_x -= HERO_OFFSET;
        float hero_y = CHero::GetInstance()->GetHeroPos_y();
        hero_y += 25;

        CTreasureChest * Chest= (CTreasureChest *)*it;
        if(Chest->getActive()) //If it is active it will check
        {
            Chest->Powerup_Animation->Update(dt);
            if(((Chest->getPositionX() - hero_x) * (Chest->getPositionX() - hero_x)) + (Chest->getPositionY() - hero_y) * (Chest->getPositionY() - hero_y) <= CollisionRange) // Collision check
            {
                if(Chest->getType() == Chest->POWERUP_HEALTH)
                {
                    if(CHero::GetInstance()->Gethero_HP() != 100) // If not 100 goes in here
                    {
                        if(CHero::GetInstance()->Gethero_HP() < 100 && CHero::GetInstance()->Gethero_HP() > 75) // if Health is in between 75 to 100, set it to 100
                        {
                            CHero::GetInstance()->setHero_Health(100);
							//soundplayer.playSounds(soundplayer.HEALTH);
                        }
                        else if(CHero::GetInstance()->Gethero_HP() <= 75) // If health is lower or equals to 75, health plus 25.
                        {
                            CHero::GetInstance()->setHero_Health(CHero::GetInstance()->Gethero_HP() + 25);
							//soundplayer.playSounds(soundplayer.HEALTH);
                        }
                    }
					soundplayer.playSounds(soundplayer.HEALTH);
                }
				if(Chest->getType() == Chest->POWERUP_SCORE)
				{
					soundplayer.playSounds(soundplayer.COIN);
				}
				if(Chest->getType() == Chest->POWERUP_ENERGY)
				{
					soundplayer.playSounds(soundplayer.POWER_UP);
				}
				if(Chest->getType() == Chest->POWERUP_SHURIKEN)
				{
					soundplayer.playSounds(soundplayer.POWER_UP);
				}
                if(Chest->getType() == Chest->POWERUP_SHURIKEN)
                {
                    CHero::GetInstance()->getInventory().IncrementShuriken();   //Adds a shuriken
                }

                Chest->SetActive(false); // Despawn the power up
            }
        }
    }
}
void StudioProject::UpdateInput(double dt)
{
	float tempHeroPos_x = CHero::GetInstance()->GetHeroPos_x();	//Hero current position X
    float tempHeroPos_y = CHero::GetInstance()->GetHeroPos_y();	//Hero current position Y
    int checkPosition_X = (int)((CHero::GetInstance()->GetMapOffset_x() + tempHeroPos_x) / m_cMap->GetTileSize());	//Hero tile position X
    int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (tempHeroPos_y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());	//Hero tile position Y
    int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
    int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (tempHeroPos_y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());
    
    if(HeroTileCheck(m_cMap, TILE_FINISH,true,true,true,true,tempHeroPos_x,tempHeroPos_y,CHero::GetInstance()->GetMapOffset_x(),CHero::GetInstance()->Getjumpspeed()) == true)
    {
        if(GetorNot == false)
        {
            std::ostringstream ss1;
            //ss1.precision(5);
            ss1 << "You have yet to collect the objective of the level!";
            RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 10, 2, 6);
        }
        else
        {
            //Changing levels after completing one
            //Still need to set that if last level stop ++ current level
            m_CurrentLevel++;
            LoadMap(m_CurrentLevel);
            LoadEnemies(m_CurrentLevel);
            LoadConsumables(m_CurrentLevel);
            CHero::GetInstance()->HeroInit(25,500);
            CHero::GetInstance()->Gethero_HP() = 100;
            CHero::GetInstance()->Gethero_EP() = 0;
            CHero::GetInstance()->setMapOffset_x(0);
            CHero::GetInstance()->setMapOffset_y(0);
        }
    }

    //Movement
    if(Application::IsKeyPressed('W'))	//Interact
    {
        if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_METALCORNER)
        {
            Transiting = true;
            MapTransition->m_anim->animActive = true;
        }
        
        //====OBJECTIVE INPUT=====//
        if(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] == TILE_OBJECTIVE)
        {
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] = 0;
            GetorNot = true;
        }
		if(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] == TILE_OBJECTIVE)
        {
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] = 0;
            GetorNot = true;
        }
        if(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 1] == TILE_OBJECTIVE)
        {
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 1] = 0;
            GetorNot = true;
        }
		if(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 2] == TILE_OBJECTIVE)
        {
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 2] = 0;
            GetorNot = true;
        }
        //=======================//
        if( 
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 2]  == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 2] == TILE_DOOR 
           )
        {
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 2]  = 0;
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] = 0;
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 2] = 0;
        }

		 if( 
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1]  == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 1] == TILE_DOOR 
           )
        {
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1]  = 0;
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] = 0;
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X + 1] = 0;
        }

        if(
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X - 1]  == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 1] == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X - 1] == TILE_DOOR 
           )
        {
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X -1]  = 0;
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X -1] = 0;
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X -1] = 0;
        }
		if(
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X - 2]  == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 2] == TILE_DOOR ||
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X - 2] == TILE_DOOR 
           )
        {
            m_cMap->theScreenMap[checkPosition_Y][checkPosition_X -2]  = 0;
            m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X -2] = 0;
            m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X -2] = 0;
        }
        //LASER SWITCH
        if(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 2] == TILE_LASER_SWITCH || 
			m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X + 1] == TILE_LASER_SWITCH || 
			m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X - 1] == TILE_LASER_SWITCH ||
			m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X-2] == TILE_LASER_SWITCH )
        {
            int m = 0;
            for(int i = 0; i < m_cMap->GetNumOfTiles_Height(); ++i)
            {
                for(int k = 0; k < m_cMap->GetNumOfTiles_Width()+1; k ++)
                {
                    m = tileOffset_x + k;
                    // If we have reached the right side of the Map, then do not display the extra column of tiles.
                    if ( (tileOffset_x+k) >= m_cMap->getNumOfTiles_MapWidth() )
                        break;
                    if( m_cMap->theScreenMap[i][m] == TILE_LASER_HORIZONTAL || m_cMap->theScreenMap[i][m] == TILE_LASER_VERTICAL)
                    {
                        m_cMap->theScreenMap[i][m] = 0;
                    }
                }

            }
        }

    }
	//Save
	if(Application::IsKeyPressed(VK_F11))
	{
		//CHero::GetInstance()->HeroMoveUpDown(false , 1.0f);
		GameMenu.setPauseState(true);
	}
	if(Application::IsKeyPressed(VK_F10))
	{
		GameMenu.setWinState(true);
	}
	//Walk Left
	if(Application::IsKeyPressed('A') && !CHero::GetInstance()->getInventory().getGrapple().getHooked() &&
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_run_right->m_anim->animActive == false)
	{
		CHero::GetInstance()->HeroMoveLeftRight(true , 1.0f);
		CHero::GetInstance()->Hero_run_left->m_anim->animActive = true;
		if(CHero::GetInstance()->Gethero_face_left() != true)
		{
			CHero::GetInstance()->Gethero_face_right() = false;
			CHero::GetInstance()->Gethero_face_left() = true;
		}
	}
	else
	{
		CHero::GetInstance()->Hero_run_left->m_anim->animActive = false;
	}
	//Walk Right
	if(Application::IsKeyPressed('D') && !CHero::GetInstance()->getInventory().getGrapple().getHooked() &&
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_run_left->m_anim->animActive == false)
	{
		CHero::GetInstance()->HeroMoveLeftRight(false , 1.0f);
		CHero::GetInstance()->Hero_run_right->m_anim->animActive = true;
		if(CHero::GetInstance()->Gethero_face_right() != true)
		{
			CHero::GetInstance()->Gethero_face_left() = false;
			CHero::GetInstance()->Gethero_face_right() = true;
		}
	}
	else
	{
		CHero::GetInstance()->Hero_run_right->m_anim->animActive = false;
	}

     //--------Physics------
    if(!Application::IsKeyPressed('D') && !Application::IsKeyPressed('A')) 
    {
        if(CHero::GetInstance()->getHero_Velocity() > 0)
        {
            float temp = CHero::GetInstance()->getHero_Velocity();
            temp -= f_deceleration * dt;
            if(temp < 0)
            {
                temp = 0;
            }
            CHero::GetInstance()->setHero_Deceleration(false,temp);

        }
        if(CHero::GetInstance()->getHero_Velocity() < 0)
        {
            float temp = CHero::GetInstance()->getHero_Velocity();
            temp += f_deceleration * dt;
            if(temp > 0)
            {
                temp = 0;
            }
            CHero::GetInstance()->setHero_Deceleration(true,temp);

        }   
    }

	//JUMPING
	jump_input_delay += (float)dt;
	if(Application::IsKeyPressed(VK_SPACE) && jump_input_delay > 0.3f &&!CHero::GetInstance()->getInventory().getGrapple().getHooked() )
	{
		CHero::GetInstance()->Getjumpcount()++;
		jump_input_delay = 0.f;
		CHero::GetInstance()->HeroJump();
	}

	/*
		Clicking
		Grappling Hook
	*/
	//Used for calculating mouse position on the screen
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x)  * 800/ w;
	posX += CHero::GetInstance()->GetMapOffset_x();
	float posY = (h - static_cast<float>(y)) * 600 / h;

	//Left mouse click controlling shuriken throws
	static bool bLButtonState = false;
	if( !bLButtonState && Application::IsMousePressed(0) )
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		soundplayer.playSounds(soundplayer.SHURIKENSOUND);
		CHero::GetInstance()->HeroThrowShuriken(meshList[GEO_SHURIKEN],posX,posY);
	}
	else if( bLButtonState && !Application::IsMousePressed(0) )
	{
		bLButtonState = false;
	}

	//Right mouse click, controls grappling hook
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1) 
		&& CHero::GetInstance()->getInventory().getGrapple().getActive() == false 
		&& CHero::GetInstance()->getInventory().getGrapple().getHooked() == false
		&& CHero::GetInstance()->Gethero_EP() >= 30)
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		CHero::GetInstance()->HeroUseGrapple(meshList[GEO_GRAPPLING_HOOK],posX,posY);
		CHero::GetInstance()->Gethero_EP() -= 30;
	}
	else if(bRButtonState && !Application::IsMousePressed(0))
	{
		bRButtonState = false;
	}

	//If grappling hook has collided with something
	if(CHero::GetInstance()->getInventory().getGrapple().getHooked())
	{
		//Moves hero towards it, if true is returned, hero has reached destination or collided with something
		if( CHero::GetInstance()->HeroGrapple( m_cMap,CHero::GetInstance()->getInventory().getGrapple().getDir(),
			CHero::GetInstance()->getInventory().getGrapple().getPos() ) )
		{
			//Hooked status will return to false
			CHero::GetInstance()->getInventory().getGrapple().setHooked(false);
		}
	}
}
void StudioProject::UpdateMap(double dt)
{
	MapTransition = dynamic_cast<CSpriteAnimation*>(meshList[MAP_TRANSITION]);
	static bool Loaded = false;
	if(Transiting == true)
	{
		MapTransition->Update(dt);
		if(MapTransition->m_anim->animActive == false)
		{
			Transiting = false;
			Loaded = false;
		}
	}
	//******************************************************************************//
	//			USED TO CONTROL TRANSITION FROM MAP TO MAP BASED ON CURRENT LEVEL	//
	//******************************************************************************//
	//if(MapTransition->m_anim->animCurrentFrame == 5 && Loaded == false)
	//{
	//	LoadEnemies(m_CurrentLevel);
	//}

	//for(unsigned i = 0; i < enemyContainer.size(); ++i)
	//{
	//	if(enemyContainer[i]->getActive() == true)
	//	{
	//		if(m_CurrentLevel == 1)
	//		{
	//			Lv1Clear = false;
	//		}
	//	}
	//	else if( i == (enemyContainer.size() - 1) )
	//	{
	//		if(m_CurrentLevel == 1)
	//		{
	//			Lv1Clear = true;
	//		}
	//	}
	//}

	if(Lv1Clear)
	{
		GameMenu.setWinState(true);
		Reset(true);
	}

	
}

void StudioProject::Update(double dt)
{
	//Money.SetCash(Score);
	if(GameMenu.getReset() == true)
	{ 
		Reset(false);
	}
	if(GameMenu.getMenuState() == true || GameMenu.getLostState() == true )
	{	
		int lvl = GameMenu.Update(dt);
		if(lvl != 0)
		{
			m_CurrentLevel = lvl;
			LoadMap(m_CurrentLevel);
		}
		//else if(lvl == 0)
		//{
		//	//cout<<"NO LEVEL SELECTED"<<endl;
		//}
		soundplayer.playSounds(soundplayer.MENU_BGM);

		if( (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed(VK_RETURN)) && GameMenu.inputDelay == 0.f)
		{
			soundplayer.playSounds(soundplayer.MENU_FEEDBACK);
		}
	}
	if(GameMenu.getWinState() == true)
	{
		int lvl = GameMenu.Update(dt);
		if(lvl != 0)
		{
			m_CurrentLevel = lvl;
			LoadMap(m_CurrentLevel);
		}
		if(GameMenu.getHealthBought() == true)
		{
			//if(GameMenu.AddHealth())//The score
			//{
				if(CHero::GetInstance()->Gethero_HP() < 100 && CHero::GetInstance()->Gethero_HP() > 75) // if Health is in between 75 to 100, set it to 100
				{
					cout<<"HEALTH++";
					CHero::GetInstance()->setHero_Health(100);
					//soundplayer.playSounds(soundplayer.HEALTH);
				}
				else if(CHero::GetInstance()->Gethero_HP() <= 75) // If health is lower or equals to 75, health plus 25.
				{
					cout<<"HEALTH++";
					CHero::GetInstance()->setHero_Health(CHero::GetInstance()->Gethero_HP() + 25);
					//soundplayer.playSounds(soundplayer.HEALTH);
				}
				soundplayer.playSounds(soundplayer.HEALTH);
			//}
			
			GameMenu.setHealthBought(false);
		}
		else if(GameMenu.getWeaponBought() == true)
		{
			//if(GameMenu.AddWeapon())//The score
			//{
				cout<<"WEAPON++";
				CHero::GetInstance()->getInventory().IncrementShuriken();  
				soundplayer.playSounds(soundplayer.POWER_UP);
			//}
			GameMenu.setWeaponBought(false);

		}
	}
	if(GameMenu.getPauseState() == true)
	{
		int lvl = GameMenu.Update(dt);
		if(lvl != 0)
		{
			m_CurrentLevel = lvl;
			LoadMap(m_CurrentLevel);
		}
		if(GameMenu.getSave() == true)
		{
			Save = true; 
			SaveGame();
			GameMenu.setSave(false);
		}
		if(GameMenu.getSavenQuit() == true)
		{
			Save = true; 
			SaveGame();
			GameMenu.setSavenQuit(false);
			exit(0);
			
		}
		if(GameMenu.getQuit() == true)
		{
			GameMenu.setQuit(false);
			exit(0);
		}
	}

	if(GameMenu.getLostState() == true )
	{
		soundplayer.playSounds(soundplayer.GAMEOVER);
		GameMenu.setLostSound(true);
		
	}
	//if(GameMenu.getLostSound() == true)
	//{
	//	soundplayer.stopSound();
	//	GameMenu.setLostSound(false);
	//}

	else if(GameMenu.getMenuState() == false && GameMenu.getLostState() == false && GameMenu.getWinState() == false)
	{
		if(GameMenu.getLoadingLevels() == true)
		{
			load = true;
			GameMenu.setLoadingLevels(false);
		}

		LoadHero();
		/*=================
		GAME START
		===================*/
		UpdateDebug(dt);
		if(!Transiting)	//If player is transiting from map to map, no input is taken in
		{
			UpdateInput(dt);
		}

		UpdateMap(dt);
		HeroUpdate(dt);
		EnemyUpdate(dt);

        UpdatePowerUp(dt);
		UpdateWeapon();
		UpdateSprites(dt);
		UpdateEnemySprites(dt);

		soundplayer.playSounds(soundplayer.GAME_BGM);

		load = false;
		//camera.Update(dt);

		fps = (float)(1.f / dt);

        //cout << Shuriken_Number << endl;
	}
    
/*
    theArrayOfGoodies = new CGoodies*[10];
	for(unsigned i = 0; i < 10; ++i)
	{
		theArrayOfGoodies[i] = theGoodiesFactory.Create(CGoodiesFactory::TREASURE_CHEST);
		theArrayOfGoodies[i]->SetPos(150 + i*25, 150);
		theArrayOfGoodies[i]->SetMesh(MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1,1,1), 0,0,25,25));
		theArrayOfGoodies[i]->SetTextureID(LoadTGA("Image//Tiles//tile4_treasurechest.tga"));
	}
    CHero::GetInstance()->GetHeroPos_x() ;
    */
}

void StudioProject::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
		temp += (textWidth[text[i]] / 100);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
void StudioProject::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		temp += textWidth[text[i]] / widthDivide;
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
void StudioProject::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for(int i = 0; i < MAX_TEXTURE; ++i)
	{
		if(mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
	}

	mesh->Render();
	//if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void StudioProject::RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeY,float sizeX, float x, float y,bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeY, sizeX, 1.f);
	

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
void StudioProject::RenderEnemyOnMap2D(Mesh *mesh, bool enableLight, float sizeY,float sizeX, float x, float y,bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.Translate(69.f,49.f,0.f);	//Map offset
	
	modelStack.PushMatrix();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeY, sizeX, 1.f);

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
void StudioProject::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate,float rotation)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				if(rotate)
				{
					modelStack.Rotate(rotation,0,0,1);
				}
				modelStack.Scale(size, size, size);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				for(int i = 0; i < MAX_TEXTURE; ++i)
				{
					if(mesh->textureArray[i] > 0)
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
					}
					else
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
					}

					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
					glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
				}

				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}

				/*else
				{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}*/
				mesh->Render();
				//if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}

			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void StudioProject::Render2DSprite(Mesh *mesh, bool enableLight, float sizex, float sizey, float x, float y, bool rotate)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(sizex, sizey, sizex);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				for(int i = 0; i < MAX_TEXTURE; ++i)
				{
					if(mesh->textureArray[i] > 0)
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
					}
					else
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
					}

					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
					glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
				}

				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}

				/*else
				{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}*/
				mesh->Render();
				//if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}

			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void StudioProject::RenderWeapon()
{  
	if(CHero::GetInstance()->getInventory().getShurikenList().size() > 0)
	{
		for(unsigned i = 0;i < CHero::GetInstance()->getInventory().getShurikenList().size(); ++i)
		{
			if(CHero::GetInstance()->getInventory().getShurikenList().at(i)->getActive() == true)
			{
				Render2DMesh(CHero::GetInstance()->getInventory().getShurikenList().at(i)->getMesh(),false,1.f,
					CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().x,
					CHero::GetInstance()->getInventory().getShurikenList().at(i)->getPos().y);
			}
		}
	}
  
	if(CHero::GetInstance()->getInventory().getGrapple().getActive())
	{
		Render2DMesh( CHero::GetInstance()->getInventory().getGrapple().getMesh(),false,1.f,CHero::GetInstance()->getInventory().getGrapple().getPos().x,
			CHero::GetInstance()->getInventory().getGrapple().getPos().y,true,
			CHero::GetInstance()->getInventory().getGrapple().getRotation() );
	}
}
void StudioProject::RenderMenu(int input)
{
	//Render background
	Render2DMesh(meshList[GEO_MENU_BACKGROUND],false);

	switch(input)
	{
	case CMenuClass::PLAY_GAME:
		{
			if(GameMenu.getMenuState())
			{
				Render2DMesh(meshList[GEO_PLAY_TEXT],false,0.8f,350,350);
				Render2DMesh(meshList[GEO_CONTROLS_TEXT],false,0.8f,265,295);
				Render2DMesh(meshList[GEO_LOAD_TEXT],false,0.8f,350,240);
				Render2DMesh(meshList[GEO_LEVEL_TEXT],false,1.f,200,185);

				Render2DMesh(meshList[GEO_MENU_FEEDBACK],false,1.f,GameMenu.getCurrentSelectPos_X(),GameMenu.getCurrentSelectPos_Y());
				RenderTextOnScreen(meshList[GEO_TEXT], "Press Escape To Exit", Color(0.3f, 0.3f, 0.3f), 2.f, 32, 10);
			}
		}
		break;
	case CMenuClass::CONTROLS:
		{
			if(GameMenu.getMenuState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "W - Go in portals", Color(1, 1, 1), 2.f, 15, 42);
				RenderTextOnScreen(meshList[GEO_TEXT], "A - Move Left", Color(1, 1, 1), 2.f, 15, 38);
				RenderTextOnScreen(meshList[GEO_TEXT], "D - Move Right", Color(1, 1, 1), 2.f, 15, 34);
				RenderTextOnScreen(meshList[GEO_TEXT], "Z - Heavy Attack", Color(1, 1, 1), 2.f, 15, 26);
				RenderTextOnScreen(meshList[GEO_TEXT], "X - Light Attack", Color(1, 1, 1), 2.f, 15, 23);
				RenderTextOnScreen(meshList[GEO_TEXT], "C - Shockwave Attack", Color(1, 1, 1), 2.f, 15, 20);
		
				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to return to main menu", Color(0.3f, 0.3f, 0.3f), 1.5f, 28, 10);
			}
		}
		break;
			case CMenuClass::LOAD:
		{
			if(GameMenu.getMenuState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "LOAD PREVIOUS GAME STATE", Color(1, 1, 1), 2.f, 15, 42);
				RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1), 2.f, 15, 38);
				RenderTextOnScreen(meshList[GEO_TEXT], "D - Move Right", Color(1, 1, 1), 2.f, 15, 34);

		
				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to return to main menu", Color(0.3f, 0.3f, 0.3f), 1.5f, 28, 10);
			}
		}
		break;
	case CMenuClass::LEVEL_SELECTION:
		{
			if(GameMenu.getMenuState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "CHOOSE LEVEL", Color(1, 1, 1), 2.f, 15, 42);
				RenderTextOnScreen(meshList[GEO_TEXT], "LEVEL 1", Color(1, 1, 1), GameMenu.getLevel1Size(), 15, 38);
				RenderTextOnScreen(meshList[GEO_TEXT], "LEVEL 2", Color(1, 1, 1), GameMenu.getLevel2Size(), 15, 34);
				RenderTextOnScreen(meshList[GEO_TEXT], "LEVEL 3", Color(1, 1, 1), GameMenu.getLevel3Size(), 15, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "LEVEL 4", Color(1, 1, 1), GameMenu.getLevel4Size(), 15, 26);
				RenderTextOnScreen(meshList[GEO_TEXT], "LEVEL 5", Color(1, 1, 1), GameMenu.getLevel5Size(), 15, 22);

				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to choose level", Color(0.3f, 0.3f, 0.3f), 1.5f, 28, 10);
			}
		}
		break;
	case CMenuClass::LOSE_SCREEN:
		{
			if(GameMenu.getLostState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Game Over!", Color(1, 0, 0), 5.f, 33, 35);

				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to return to main menu", Color(0.3f, 0.3f, 0.3f), 1.5f, 32, 10);
			}
		}
		break;
	case CMenuClass::WIN_SCREEN:
		{
			if(GameMenu.getWinState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "WELCOME TO THE SHOP", Color(1, 1, 1), 3.f, 15, 42);
				RenderTextOnScreen(meshList[GEO_TEXT], "+HEALTH($5)   ", Color(1, 1, 1), GameMenu.getHealthSize(), 15, 38);
				RenderTextOnScreen(meshList[GEO_TEXT], "+SHURIKEN($10)", Color(1, 1, 1), GameMenu.getWeaponSize(), 15, 34);
				RenderTextOnScreen(meshList[GEO_TEXT], "RETURN", Color(0.3f, 0.3f, 0.3f), GameMenu.getOutSize(), 28, 10);
			}
		}
	case CMenuClass::PAUSE:
		{
			if(GameMenu.getPauseState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "PAUSE SCREEN", Color(0.3f, 0.3f, 0.3f), 3.f, 28, 42);
				RenderTextOnScreen(meshList[GEO_TEXT], "SAVE   ", Color(1, 1, 1), GameMenu.getSaveSize(), 28, 38);
				RenderTextOnScreen(meshList[GEO_TEXT], "SAVE AND QUIT", Color(1, 1, 1), GameMenu.getSQSize(), 28, 34);
				RenderTextOnScreen(meshList[GEO_TEXT], "QUIT", Color(1, 1, 1), GameMenu.getQuitSize(), 28, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "RETURN TO GAME", Color(1, 1, 1), GameMenu.getReturnSize(), 28, 26);
				RenderTextOnScreen(meshList[GEO_TEXT], "RETURN TO MAIN MENU", Color(1, 1, 1), GameMenu.getReturnMainSize(), 28, 24);
			}
		}
		break;

		
	}
}
void StudioProject::RenderHUD(void)
{ 
	//ICON
	//RenderMeshIn2D(meshList[GEO_ICON],false,0.7f,0.7f,-69.f,51.f,false);

	//HUD template
	RenderMeshIn2D(meshList[GEO_HUD_TEMPLATE],false,2.f,2.f,-57.f,51.f,false);
	//Health points
	RenderMeshIn2D(meshList[GEO_HUD_HP],false,0.032f * CHero::GetInstance()->Gethero_HP(),2.f,-69.8f,53.7f,false);
	//Energy points
	RenderMeshIn2D(meshList[GEO_HUD_EP],false,0.032f * CHero::GetInstance()->Gethero_EP(),2.f,-69.8f,48.1f,false);

	//Shuriken Count
	//RenderMeshIn2D(meshList[GEO_SHURIKEN],false,0.5f,0.5f,0.f,53.7f,false);
	//RenderMeshIn2D(meshList[GEO_HEALTHBAR],false,player.getHitpoints() * 0.025f,0.3f,-79.0f,-57.5f,false);

	/*std::ostringstream ss3;
	ss3.precision(5);
	ss3 << player.getHitpoints();
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 0, 0), 3, 8, 3.7f);*/

	////Score
	//std::ostringstream ss6;
	//ss6.precision(5);
	//ss6 << "Score: " << score;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 0, 1), 3, 30, 2.5f);

	//===================================================//
	//Debug print
	//===================================================//
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 59);
}
void StudioProject::RenderBackground(void)
{
	Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f);

	rearWallOffset_x = (int) (CHero::GetInstance()->GetMapOffset_x() * 0.5);
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

	//Render2DMesh(meshList[GEO_LAYER_2], false, 1.0f,(float)(100 - rearWallOffset_x));
	//Render2DMesh(meshList[GEO_LAYER_2], false, 1.0f,(float)(1000 - rearWallOffset_x));

	rearWallOffset_x = (int) (CHero::GetInstance()->GetMapOffset_x() * 0.25);
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

	//Render2DMesh(meshList[GEO_LAYER_3], false, 1.0f,(float)(100 - rearWallOffset_x));
	//Render2DMesh(meshList[GEO_LAYER_3], false, 1.0f,(float)(1000 - rearWallOffset_x));
}
void StudioProject::RenderPowerUp(void)
{
    for(std::vector<CTreasureChest *>::iterator it = Treasure.begin(); it != Treasure.end(); ++it)
    {
        CTreasureChest * Chest = (CTreasureChest *)*it;
        if(Chest->getActive())
        {
            Render2DSprite(Chest->Powerup_Animation,false,Chest->Powerup_Animation->m_anim->animScale,Chest->Powerup_Animation->m_anim->animScale,Chest->Powerup_Animation->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x(),Chest->Powerup_Animation->m_anim->animPosition.y,false);
        }
    }
}
/********************************************************************************
 Render the tile map. This is a private function for use in this class only
 ********************************************************************************/
void StudioProject::RenderTileMap()
{
	int m = 0;
	for(int i = 0; i < m_cMap->GetNumOfTiles_Height(); ++i)
	{
		for(int k = 0; k < m_cMap->GetNumOfTiles_Width()+1; k ++)
		{
			m = tileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ( (tileOffset_x+k) >= m_cMap->getNumOfTiles_MapWidth() )
				break;
			if (m_cMap->theScreenMap[i][m] == TILE_METALFLOOR)
			{
				Render2DMesh(meshList[GEO_TILE_METALFLOOR], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == TILE_METALSUPPORT_RIGHT)
			{
				Render2DMesh(meshList[GEO_TILE_METALSUPPORT_RIGHT], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == TILE_METALSUPPORT_LEFT)
			{
				Render2DMesh(meshList[GEO_TILE_METALSUPPORT_LEFT], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == TILE_METALCORNER)
			{
				Render2DMesh(meshList[GEO_TILE_METALCORNER], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
            else if (m_cMap->theScreenMap[i][m] == TILE_DOOR)
            {
                Render2DMesh(meshList[GEO_TILE_DOOR], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_HORIZONTAL)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_HORIZONTAL], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_VERTICAL)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_VERTICAL], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_SWITCH)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_SWITCH], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_HEALTH)
            {
                Render2DMesh(meshList[GEO_TILE_HEALTH], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_SCORE)
            {
                Render2DMesh(meshList[GEO_TILE_SCORE], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_SHURIKEN)
            {
                Render2DMesh(meshList[GEO_TILE_SHURIKEN], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_OBJECTIVE)
            {
                Render2DMesh(meshList[GEO_TILE_OBJECTIVE], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_SHOOTER_UP)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_SHOOTER_UP], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_SHOOTER_DOWN)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_SHOOTER_DOWN], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_SHOOTER_LEFT)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_SHOOTER_LEFT], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_LASER_SHOOTER_RIGHT)
            {
                Render2DMesh(meshList[GEO_TILE_LASER_SHOOTER_RIGHT], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_STEALTH_BOX)
            {
                Render2DMesh(meshList[GEO_TILE_STEALTH_BOX], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == TILE_FINISH)
            {
                if(GetorNot == false)
                    Render2DMesh(meshList[GEO_TILE_FINISH_CLOSE], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
                else
                    Render2DMesh(meshList[GEO_TILE_FINISH_OPEN], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
            }
		}
	}
}
void StudioProject::RenderRearTileMap()
{
	rearWallOffset_x = (int) (CHero::GetInstance()->GetMapOffset_x()/ 2);
	rearWallOffset_y = 0;
	rearWallTileOffset_y = 0;
	rearWallTileOffset_x = (int) (rearWallOffset_x / m_cRearMap->GetTileSize());
	if(rearWallTileOffset_x + m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->getNumOfTiles_MapWidth())
	{
		rearWallTileOffset_x += m_cRearMap->getNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
	}
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();
	
	int m = 0;

	for(int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i ++)
	{
		for(int k = 0; k < m_cRearMap->GetNumOfTiles_Width()+1; k ++)
		{
			m = rearWallTileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ( (rearWallTileOffset_x+k) >= m_cRearMap->getNumOfTiles_MapWidth() )
				break;
			if (m_cRearMap->theScreenMap[i][m] == WINDOW_TOP)
			{
				Render2DMesh(meshList[GEO_WINDOW_TOP], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
			else if (m_cRearMap->theScreenMap[i][m] == WINDOW_LEFT)
			{
				Render2DMesh(meshList[GEO_WINDOW_LEFT], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
			else if (m_cRearMap->theScreenMap[i][m] == WINDOW_RIGHT)
			{
				Render2DMesh(meshList[GEO_WINDOW_RIGHT], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
			else if (m_cRearMap->theScreenMap[i][m] == WINDOW_BOTTOM)
			{
				Render2DMesh(meshList[GEO_WINDOW_BOTTOM], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
			else if (m_cRearMap->theScreenMap[i][m] == REAR_WALL)
			{
				Render2DMesh(meshList[GEO_REAR_WALL], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
			else if (m_cRearMap->theScreenMap[i][m] == REAR_GLASS)
			{
				Render2DMesh(meshList[GEO_REAR_GLASS], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
			}
		}
	}
}
void StudioProject::RenderDebug(void)
{
	//RenderMesh(meshList[GEO_AXES], false);
	//std::ostringstream ss;
	//ss.precision(5);
	//ss << "tileOffset_x: " << tileOffset_x;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 4);
	//std::ostringstream sss;
	//sss.precision(5);
	//sss << "mapOffset_x: "<<CHero::GetInstance()->GetMapOffset_x();
	//RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 3, 2, 7);
    
	std::ostringstream sss;
	sss.precision(5);
	sss << "heropos.x: "<< CHero::GetInstance()->GetMapOffset_x() + CHero::GetInstance()->GetHeroPos_x();
	RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 3, 2, 7);

	std::ostringstream ssss;
	ssss.precision(5);
	ssss << "heropos.y: "<<CHero::GetInstance()->GetHeroPos_y();
	RenderTextOnScreen(meshList[GEO_TEXT], ssss.str(), Color(0, 1, 0), 3, 2, 4);
    

}
void StudioProject::RenderHeroSprites(void)
{
	//===================================================================================//
	//				
	//							IDLE													//
	//
	//==================================================================================//
	if(CHero::GetInstance()->Hero_idle_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_idle_right,false,CHero::GetInstance()->Hero_idle_right->m_anim->animScale,CHero::GetInstance()->Hero_idle_right->m_anim->animScale,
			CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.x,CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	if(CHero::GetInstance()->Hero_idle_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_idle_left,false,CHero::GetInstance()->Hero_idle_left->m_anim->animScale,CHero::GetInstance()->Hero_idle_left->m_anim->animScale,
			CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.x,CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	//===================================================================================//
	//				
	//							RUN													//
	//
	//==================================================================================//
	if(CHero::GetInstance()->Hero_run_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_run_right,false,CHero::GetInstance()->Hero_run_right->m_anim->animScale,CHero::GetInstance()->Hero_run_right->m_anim->animScale,
			CHero::GetInstance()->Hero_run_right->m_anim->animPosition.x,CHero::GetInstance()->Hero_run_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	if(CHero::GetInstance()->Hero_run_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_run_left,false,CHero::GetInstance()->Hero_run_left->m_anim->animScale,CHero::GetInstance()->Hero_run_left->m_anim->animScale,
			CHero::GetInstance()->Hero_run_left->m_anim->animPosition.x,CHero::GetInstance()->Hero_run_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	//===================================================================================//
	//				
	//							JUMP													//
	//
	//==================================================================================//
	if(CHero::GetInstance()->Hero_jump_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_jump_right,false,CHero::GetInstance()->Hero_jump_right->m_anim->animScale,CHero::GetInstance()->Hero_jump_right->m_anim->animScale,
			CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.x,CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	if(CHero::GetInstance()->Hero_jump_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_jump_left,false,CHero::GetInstance()->Hero_jump_left->m_anim->animScale,CHero::GetInstance()->Hero_jump_left->m_anim->animScale,
			CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.x,CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	
}
void StudioProject::RenderEnemySprites(CEnemy* enemyInput)
{
	int enemy_x = enemyInput->GetPos_x() - CHero::GetInstance()->GetMapOffset_x();
	int enemy_y = enemyInput->GetPos_y();

	//Detection Range
	if(enemyInput->getHealth() > 0 )
	{
		Render2DMesh(meshList[GEO_GUARD_DETECTION],false,1.f,enemy_x + 25.f,enemy_y + (ENEMY_SIZE * 0.5));
	}
	//Walking right
	if(enemyInput->WalkAnimation_Right->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->WalkAnimation_Right,false,enemyInput->WalkAnimation_Right->m_anim->animScale,enemyInput->WalkAnimation_Right->m_anim->animScale,
			enemy_x + ENEMY_OFFSET,enemyInput->GetPos_y() + ENEMY_OFFSET,false);
	}
	//Walkng left
	if(enemyInput->WalkAnimation_Left->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->WalkAnimation_Left,false,enemyInput->WalkAnimation_Left->m_anim->animScale,enemyInput->WalkAnimation_Left->m_anim->animScale,
			enemy_x + ENEMY_OFFSET,enemyInput->GetPos_y() + ENEMY_OFFSET,false);
	}

	//death animation Right
	if(enemyInput->DeathAnimation_Right->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->DeathAnimation_Right,false,enemyInput->DeathAnimation_Right->m_anim->animScale,enemyInput->DeathAnimation_Right->m_anim->animScale,
			(enemyInput->DeathAnimation_Right->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->DeathAnimation_Right->m_anim->animPosition.y + ENEMY_OFFSET,false);
	}

	//death animation Left
	if(enemyInput->DeathAnimation_Left->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->DeathAnimation_Left,false,enemyInput->DeathAnimation_Left->m_anim->animScale,enemyInput->DeathAnimation_Left->m_anim->animScale,
			(enemyInput->DeathAnimation_Left->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->DeathAnimation_Left->m_anim->animPosition.y + ENEMY_OFFSET,false);
	}

	//Attack Right
	if(enemyInput->AttackAnimation_Right->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->AttackAnimation_Right,false,enemyInput->AttackAnimation_Right->m_anim->animScale,enemyInput->AttackAnimation_Right->m_anim->animScale,
			(enemyInput->AttackAnimation_Right->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->AttackAnimation_Right->m_anim->animPosition.y + ENEMY_OFFSET,false);
	}

	//Attack Left
	if(enemyInput->AttackAnimation_Left->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->AttackAnimation_Left,false,enemyInput->AttackAnimation_Left->m_anim->animScale,enemyInput->AttackAnimation_Left->m_anim->animScale,
			(enemyInput->AttackAnimation_Left->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->AttackAnimation_Left->m_anim->animPosition.y + ENEMY_OFFSET,false);
	}

	//Rendering Enemy Bullet
	for(unsigned i = 0; i < enemyInput->getEnemyBullet().size(); ++i)
	{
		if( enemyInput->getEnemyBullet().at(i)->getActive() == true)
		{
			Render2DMesh(enemyInput->getEnemyBullet().at(i)->getMesh(),false,1.f,enemyInput->getEnemyBullet().at(i)->getPos().x - CHero::GetInstance()->GetMapOffset_x(), enemyInput->getEnemyBullet().at(i)->getPos().y);
		}
	}
}
void StudioProject::RenderTransition(void)
{
	MapTransition = dynamic_cast<CSpriteAnimation*>(meshList[MAP_TRANSITION]);
	if(MapTransition->m_anim->animActive == true)
	{
		Render2DSprite(meshList[MAP_TRANSITION],false,MapTransition->m_anim->animScale,MapTransition->m_anim->animScale,MapTransition->m_anim->animPosition.x,MapTransition->m_anim->animPosition.y,false);
	}
}

// Toggle HUD mode
void StudioProject::SetHUD(const bool m_bHUDmode)
{
	if (m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

}
void StudioProject::ClearBuffer(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
}
void StudioProject::EnableCamera(void)
{
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void StudioProject::Render()
{
	ClearBuffer();
	EnableCamera();

	if(GameMenu.getMenuState() == true && GameMenu.getControlState() == false && GameMenu.getLoadingLevels() == false  && GameMenu.getChoosingLevels() == false && GameMenu.getLostState() == false)	//Rendering Default screen
	{
		RenderMenu(CMenuClass::PLAY_GAME);
	}
	else if(GameMenu.getMenuState() == true && GameMenu.getControlState() == true && GameMenu.getLoadingLevels() == false && GameMenu.getChoosingLevels() == false && GameMenu.getLostState() == false)	//Rendering Control Screen
	{
		RenderMenu(CMenuClass::CONTROLS);
	}
	else if(GameMenu.getMenuState() == true && GameMenu.getLoadingLevels() == true && GameMenu.getControlState() == false && GameMenu.getChoosingLevels() == false  && GameMenu.getLostState() == false)	//Rendering Control Screen
	{
		RenderMenu(CMenuClass::LOAD);
	}
	else if(GameMenu.getMenuState() == true && GameMenu.getLoadingLevels() == false && GameMenu.getChoosingLevels() == true && GameMenu.getControlState() == false  && GameMenu.getLostState() == false)	//Rendering Control Screen
	{
		RenderMenu(CMenuClass::LEVEL_SELECTION);
	}
	else if(GameMenu.getMenuState() == false && GameMenu.getLostState() == false && GameMenu.getWinState() == false && GameMenu.getPauseState() == false)
	{
		/*===================
			GAME START
		=====================*/
		RenderBackground();

		//RenderRearTileMap();

		RenderTileMap();
		
		RenderPowerUp();
		for(std::vector<CEnemy *>::iterator it = enemyContainer.begin(); it != enemyContainer.end(); ++it)
		{
			CEnemy *Enemy = (CEnemy *)*it;

			if(Enemy->getActive())
			{
				RenderEnemySprites(Enemy);
			}
		}

		RenderHeroSprites();
		RenderWeapon();
		RenderDebug();

		SetHUD(true);
		RenderHUD();
		SetHUD(false);

		RenderTransition();

	}
	else if(GameMenu.getLostState() == true)
	{
		RenderMenu(CMenuClass::LOSE_SCREEN);
	}
	else if(GameMenu.getWinState() == true)
	{
		RenderMenu(CMenuClass::WIN_SCREEN);
	}
	else if(GameMenu.getPauseState() == true)
	{
		RenderMenu(CMenuClass::PAUSE);
	}
}

void StudioProject::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	while(enemyContainer.size() > 0)
	{
		CEnemy *enemy = enemyContainer.back();
		delete enemy;
		enemyContainer.pop_back();
	}
    while(Treasure.size() > 0)
    {
        CTreasureChest* Chest = Treasure.back();
        delete Chest;
        Treasure.pop_back();
    }
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
