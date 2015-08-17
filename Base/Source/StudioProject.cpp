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
	rearWallFineOffset_y(0)
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
	meshList[GEO_MENU_BACKGROUND]->textureID = LoadTGA("Image//Menu//MenuBackground.tga");

	meshList[GEO_MENU_FEEDBACK] = MeshBuilder::Generate2DMesh("GameMenu_feedback", Color(1, 1, 1), 0, 0, 210, 80);
	meshList[GEO_MENU_FEEDBACK]->textureID = LoadTGA("Image//Menu//SelectionFeedback.tga");

	meshList[GEO_PLAY_TEXT] = MeshBuilder::Generate2DMesh("GameMenuPlayText", Color(1, 1, 1), 0, 0, 100, 30);
	meshList[GEO_PLAY_TEXT]->textureID = LoadTGA("Image//Menu//PlayText.tga");

	meshList[GEO_CONTROLS_TEXT] = MeshBuilder::Generate2DMesh("GameMenuControlsText", Color(1, 1, 1), 0, 0, 220, 30);
	meshList[GEO_CONTROLS_TEXT]->textureID = LoadTGA("Image//Menu//ControlsText.tga");

}
void StudioProject::InitCamera()
{
	camera.Init(Vector3(-1000, 68, 10), Vector3(-1000, 68, 0), Vector3(0, 1, 0));
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void StudioProject::InitVariables()
{

	ENEMY_SIZE = 70.f;
	ENEMY_OFFSET = 33.f;

	//Hero
	HERO_SIZE = 80.f;
	HERO_OFFSET = 33.f;
	jump_input_delay = 0.f;

	//Sound
	soundplayer.Init();

	Sound_shockwave_delay = 0.6f;
	Sound_swordswing_delay = 0.4f;

	//Level
	m_CurrentLevel = 1;
	Lv1Clear = false;

	//Map
	Transiting = false;
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
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//Backgroundtest2.tga");

	//Second layer for parrallax
	meshList[GEO_LAYER_2] = MeshBuilder::Generate2DMesh("Layer2", Color(1, 1, 1), 0, 0, 900, 600);
	meshList[GEO_LAYER_2]->textureID = LoadTGA("Image//Backgroundlayer2.tga");

	//Third layer for parrallax
	meshList[GEO_LAYER_3] = MeshBuilder::Generate2DMesh("Layer3", Color(1, 1, 1), 0, 0, 900, 600);
	meshList[GEO_LAYER_3]->textureID = LoadTGA("Image//Backgroundlayer3.tga");
}
void StudioProject::InitHero()
{
	CHero::GetInstance()->HeroInit(0,500);

	//Sprite Animation
	//===================================================================================//
	//				
	//							IDLE													//
	//
	//==================================================================================//
	meshList[GEO_SABER_IDLE_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_idle_left",1,2);
	meshList[GEO_SABER_IDLE_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_idle_right.tga");
	
	CHero::GetInstance()->Hero_idle_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_IDLE_RIGHT]);
	if(CHero::GetInstance()->Hero_idle_right)
	{
		CHero::GetInstance()->Hero_idle_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_idle_right->m_anim->Set(0, 1, 1, 1.f, true,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	meshList[GEO_SABER_IDLE_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_idle_left",1,2);
	meshList[GEO_SABER_IDLE_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_idle_left.tga");
	
	CHero::GetInstance()->Hero_idle_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_IDLE_LEFT]);
	if(CHero::GetInstance()->Hero_idle_left)
	{
		CHero::GetInstance()->Hero_idle_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_idle_left->m_anim->Set(0, 1, 1, 1.f, true,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	//===================================================================================//
	//				
	//							RUN													//
	//
	//==================================================================================//

	meshList[GEO_SABER_RUN_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_run_right",1,8);
	meshList[GEO_SABER_RUN_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_run_right.tga");
	
	CHero::GetInstance()->Hero_run_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_RUN_RIGHT]);
	if(CHero::GetInstance()->Hero_run_right)
	{
		CHero::GetInstance()->Hero_run_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_run_right->m_anim->Set(0, 7, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}

	meshList[GEO_SABER_RUN_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_run_left",1,8);
	meshList[GEO_SABER_RUN_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_run_left.tga");
	
	CHero::GetInstance()->Hero_run_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_RUN_LEFT]);
	if(CHero::GetInstance()->Hero_run_left)
	{
		CHero::GetInstance()->Hero_run_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_run_left->m_anim->Set(0, 7, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
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
	
	meshList[GEO_SABER_JUMP_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Saber_jump_right",1,1);
	meshList[GEO_SABER_JUMP_RIGHT]->textureArray[0] = LoadTGA("Image//Saber//Saber_jump_right.tga");

	CHero::GetInstance()->Hero_jump_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_JUMP_RIGHT]);
	if(CHero::GetInstance()->Hero_jump_right)
	{
		CHero::GetInstance()->Hero_jump_right->m_anim = new Animation();
		CHero::GetInstance()->Hero_jump_right->m_anim->Set(0, 0, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}
	
	meshList[GEO_SABER_JUMP_LEFT] = MeshBuilder::GenerateSpriteAnimation("Saber_jump_left",1,1);
	meshList[GEO_SABER_JUMP_LEFT]->textureArray[0] = LoadTGA("Image//Saber//Saber_jump_left.tga");

	CHero::GetInstance()->Hero_jump_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_JUMP_LEFT]);
	if(CHero::GetInstance()->Hero_jump_left)
	{
		CHero::GetInstance()->Hero_jump_left->m_anim = new Animation();
		CHero::GetInstance()->Hero_jump_left->m_anim->Set(0, 0, 1, 1.f, false,Vector3(0,0,1),0,0.f,HERO_SIZE);
	}
}
void StudioProject::InitMap()
{
	// Initialise and load the tile map
	m_cMap = new CMap();

	Level1 = new CMap();
	Level1->Init( ScreenHeight, ScreenWidth, 24, 32, 600, 1600 );
	Level1->LoadMap( "Image//MapDesigns//Level2.csv" );
	
	m_cMap = Level1;

	//Init and load rear tile map
	m_cRearMap = new CMap();
	m_cRearMap->Init (600, 800, 24, 32, 600, 1600);
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
		based on the selected col and row
	*/
	//Ground
	meshList[GEO_TILEUNDERGROUND] = MeshBuilder::GenerateTile("underground",20,16,3,1,25);
	meshList[GEO_TILEUNDERGROUND]->textureID = LoadTGA("Image//Tiles//tileset1.tga");

	meshList[GEO_TILEGRASSGROUND] = MeshBuilder::GenerateTile("grassground",20,16,2,2,25);
	meshList[GEO_TILEGRASSGROUND]->textureID = LoadTGA("Image//Tiles//tileset1.tga");

	theArrayOfGoodies = new CGoodies*[10];
	for(unsigned i = 0; i < 10; ++i)
	{
		theArrayOfGoodies[i] = theGoodiesFactory.Create(CGoodiesFactory::TREASURE_CHEST);
		theArrayOfGoodies[i]->SetPos(150 + i*25, 150);
		theArrayOfGoodies[i]->SetMesh(MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1,1,1), 0,0,25,25));
		theArrayOfGoodies[i]->SetTextureID(LoadTGA("Image//Tiles//tile4_treasurechest.tga"));
	}
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
	LoadEnemies(m_CurrentLevel);
}

void StudioProject::Reset(bool hasWon)
{
	/*
	This function resets the game's variables based on the win / lose state.
	If the player has Won the game, the game is resetted differently from when the player has lost the game
	*/
	if(hasWon == false)
	{
		CHero::GetInstance()->HeroInit(50,125);
		CHero::GetInstance()->Gethero_HP() = 100;
		CHero::GetInstance()->Gethero_EP() = 0;
		CHero::GetInstance()->setMapOffset_x(0);
		CHero::GetInstance()->setMapOffset_y(0);

		m_CurrentLevel = 1;

		Sound_shockwave_delay = 0.6f;
		Sound_swordswing_delay = 0.4f;

		//Level
		m_CurrentLevel = 1;
		Lv1Clear = false;
		LoadEnemies(m_CurrentLevel);

		//Map
		Transiting = false;
		m_cMap = Level1;

		soundplayer.stopSound();
		GameMenu.setReset(false);
	}
	if(hasWon == true)
	{
		CHero::GetInstance()->HeroInit(50,125);
		CHero::GetInstance()->Gethero_HP() = 100;
		CHero::GetInstance()->Gethero_EP() = 0;
		CHero::GetInstance()->setMapOffset_x(0);
		CHero::GetInstance()->setMapOffset_y(0);

		m_CurrentLevel = 1;

		Sound_shockwave_delay = 0.6f;
		Sound_swordswing_delay = 0.4f;

		//Level
		m_CurrentLevel = 1;
		Lv1Clear = false;
		LoadEnemies(m_CurrentLevel);

		//Map
		Transiting = false;
		m_cMap = Level1;

		soundplayer.stopSound();
	
		GameMenu.setReset(false);
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
				ifstream datafiles("Source//TextFiles//Level2Data.txt");
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
		unsigned j = 0; // used to control variables passed in
		for(unsigned i = 0; i < (positionData.size() / 5); ++i)
		{	
			if(i == 0)
			{
				j += 4;	//+3 for first case
			}
			else
			{
				j += 5;
			}
			CEnemy* enemy = new CEnemy();
			/*
			Skeleton MONSTER
			*/
			if(positionData[j] == 1)
			{
				enemy->Init(positionData[j-4],positionData[j-3],positionData[j-2],positionData[j-1],true,CEnemy::SKELETON);
				enemy->ChangeStrategy(new CStrategy_Kill());

				//Right walk
				meshList[GEO_SKELE_WALK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("skele_walk_Right",1,9);
				meshList[GEO_SKELE_WALK_RIGHT]->textureArray[0] = LoadTGA("Image//Skeleton//SkeletonWalk2_Right.tga");
				enemy->WalkAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SKELE_WALK_RIGHT]);
				if(enemy->WalkAnimation_Right)
				{
					enemy->WalkAnimation_Right->m_anim = new Animation();
					enemy->WalkAnimation_Right->m_anim->Set(1, 8, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Left walk
				meshList[GEO_SKELE_WALK_LEFT] = MeshBuilder::GenerateSpriteAnimation("skele_walk_Left",1,9);
				meshList[GEO_SKELE_WALK_LEFT]->textureArray[0] = LoadTGA("Image//Skeleton//SkeletonWalk_Left.tga");
				enemy->WalkAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SKELE_WALK_LEFT]);
				if(enemy->WalkAnimation_Left)
				{
					enemy->WalkAnimation_Left->m_anim = new Animation();
					enemy->WalkAnimation_Left->m_anim->Set(1, 8, 1, 1.f, true,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Death animation
				meshList[GEO_SKELE_DEATH_RIGHT] = MeshBuilder::GenerateSpriteAnimation("skele_death_right",1,7);
				meshList[GEO_SKELE_DEATH_RIGHT]->textureArray[0] = LoadTGA("Image//Skeleton//SkeletonDeath_Right.tga");
				enemy->DeathAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SKELE_DEATH_RIGHT]);
				if(enemy->DeathAnimation_Right)
				{
					enemy->DeathAnimation_Right->m_anim = new Animation();
					enemy->DeathAnimation_Right->m_anim->Set(0, 6, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,ENEMY_SIZE);
				}

				//Attacking animation
				meshList[GEO_SKELE_ATTACK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("skele_attack_right",1,4);
				meshList[GEO_SKELE_ATTACK_RIGHT]->textureArray[0] = LoadTGA("Image//Skeleton//SkeletonAttack_Right.tga");
				enemy->AttackAnimation_Right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SKELE_ATTACK_RIGHT]);
				if(enemy->AttackAnimation_Right)
				{
					enemy->AttackAnimation_Right->m_anim = new Animation();
					enemy->AttackAnimation_Right->m_anim->Set(0, 3, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,100.f);
				}

				meshList[GEO_SKELE_ATTACK_LEFT] = MeshBuilder::GenerateSpriteAnimation("skele_attack_left",1,4);
				meshList[GEO_SKELE_ATTACK_LEFT]->textureArray[0] = LoadTGA("Image//Skeleton//SkeletonAttack_Left.tga");
				enemy->AttackAnimation_Left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SKELE_ATTACK_LEFT]);
				if(enemy->AttackAnimation_Left)
				{
					enemy->AttackAnimation_Left->m_anim = new Animation();
					enemy->AttackAnimation_Left->m_anim->Set(0, 3, 0, 1.f, false,Vector3((float)enemy->GetPos_x(),(float)enemy->GetPos_y(),1),0,0.f,100.f);
				}

				enemyContainer.push_back(enemy);
			}
			/*
			GHOST MONSTER
			*/
			else if(positionData[j] == 2)
			{
				enemy->Init(positionData[j-4],positionData[j-3],positionData[j-2],positionData[j-1],true,CEnemy::GHOST);
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
void StudioProject::AttackResponse(CHero::ATTACK_TYPE type)
{
	/*
	This function determines what kind of response happens after the player attack is triggered
	*/
	switch(type)
	{
	case CHero::SHOCKWAVE:
		{
			soundplayer.playSounds(soundplayer.SABER_SHOCKWAVE);
			for(unsigned i = 0; i < enemyContainer.size(); ++i)
			{													//Mapping enemy position to screen
				if(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::SHOCKWAVE) != 0)
				{
					enemyContainer[i]->EnemyDamaged(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::SHOCKWAVE),m_cMap);
				}
			}
			Sound_shockwave_delay = 0.f;
		}
		break;
	case CHero::ATTACK_1:
		{
			if(CHero::GetInstance()->Gethero_EP() < 100)
			{
				CHero::GetInstance()->Gethero_EP() += 5;
			}

			soundplayer.playSounds(soundplayer.SABER_SWING);
			for(unsigned i = 0; i < enemyContainer.size(); ++i)
			{													//Mapping enemy position to screen
				if(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::ATTACK_1) != 0 && enemyContainer[i]->getActive() == true)
				{
					enemyContainer[i]->EnemyDamaged(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::ATTACK_1),m_cMap);
					soundplayer.playSounds(soundplayer.SWORD_IMPACT);
				}
			}
			Sound_swordswing_delay = 0.f;
		}
		break;
	case CHero::ATTACK_2:
		{
			if(CHero::GetInstance()->Gethero_EP() < 100)
			{
				CHero::GetInstance()->Gethero_EP() += 5;
			}

			soundplayer.playSounds(soundplayer.SABER_SWING);
			for(unsigned i = 0; i < enemyContainer.size(); ++i)
			{													//Mapping enemy position to screen
				if(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::ATTACK_2) != 0 && enemyContainer[i]->getActive() == true)
				{
					enemyContainer[i]->EnemyDamaged(CHero::GetInstance()->HeroAttack(enemyContainer[i]->GetPos_x() - CHero::GetInstance()->GetMapOffset_x(),enemyContainer[i]->GetPos_y(),CHero::ATTACK_2),m_cMap);
					soundplayer.playSounds(soundplayer.SWORD_IMPACT);
				}
			}
			Sound_swordswing_delay = 0.f;
		}
		break;
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
	int checkPosition_X = (int)((CHero::GetInstance()->GetMapOffset_x() + tempHeroPos_x) / m_cMap->GetTileSize());	//Hero's tile position X
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (tempHeroPos_y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());	//Hero's tile positiion Y

	//if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 10)	//Check if hero is in zone
	//{
	//	enemy->ChangeStrategy(new CStrategy_Kill());
	//}
	//else if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 11)	//Check if hero is in zone
	//{
	//	enemy->ChangeStrategy(NULL);
	//}
	//else
	//{
	////	enemy->ChangeStrategy(NULL);
	//}

	//Updates the enemies
	for(std::vector<CEnemy *>::iterator it = enemyContainer.begin(); it != enemyContainer.end(); ++it)
	{
		CEnemy *Enemy = (CEnemy *)*it;
		
		//Updating enemy if active
		if(Enemy->getActive())
		{
			Enemy->SetDestination((int)tempHeroPos_x + CHero::GetInstance()->GetMapOffset_x(),(int)tempHeroPos_y +CHero::GetInstance()->GetMapOffset_y());
			Enemy->Update(m_cMap,ScreenWidth,ScreenHeight,m_CurrentLevel);

			/*int enemy_x = Enemy->GetPos_x() + CHero::GetInstance()->GetMapOffset_x();
			int enemy_y = Enemy->GetPos_y();
			Enemy->SetPos_x(enemy_x);*/
		}
			//Plays sound if enemy is dead
		else if(Enemy->DeathAnimation_Right->m_anim->animActive == true)
		{
			if(Enemy->enemyType == CEnemy::SKELETON)
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
			CHero::GetInstance()->HeroDamaged(Enemy->GetPos_x(),Enemy->GetPos_y());
			if(Enemy->EnemyAttack() != 0)
			{
				CHero::GetInstance()->HeroDamaged( Enemy->EnemyAttack() );
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

	}
}
void StudioProject::HeroUpdate(double dt)
{
	CHero::GetInstance()->Update(m_cMap,ScreenWidth,ScreenHeight,m_CurrentLevel);	//Update hero

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
	//	Shockwave						//
	//=================================//

	CHero::GetInstance()->Hero_shockwave_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_shockwave_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_shockwave_right)
	{
		CHero::GetInstance()->Hero_shockwave_right->Update(dt);
		//Response happens after a certain delay
		if(Sound_shockwave_delay >= 0.6f && CHero::GetInstance()->Hero_shockwave_right->m_anim->animeCurrentTime >= 0.4f)
		{
			AttackResponse(CHero::SHOCKWAVE);
		}
	}

	CHero::GetInstance()->Hero_shockwave_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_shockwave_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_shockwave_left)
	{
		CHero::GetInstance()->Hero_shockwave_left->Update(dt);
		//Response happens after a certain delay
		if(Sound_shockwave_delay >= 0.6f && CHero::GetInstance()->Hero_shockwave_left->m_anim->animeCurrentTime >= 0.4f) //Plays sound after certain time
		{
			AttackResponse(CHero::SHOCKWAVE);
		}
	}

	//===================================//
	//	Attack 1						//
	//=================================//

	//Sound needs changing after enemy added
	CHero::GetInstance()->Hero_attack_1_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_attack_1_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_attack_1_right)
	{
		CHero::GetInstance()->Hero_attack_1_right->Update(dt);
		//Response happens after a certain delay
		if(CHero::GetInstance()->Hero_attack_1_right->m_anim->animeCurrentTime >= 0.69f)
		{
			if(Sound_swordswing_delay >= 0.2f)
			{
				AttackResponse(CHero::ATTACK_1);
			}
		}
	}
	CHero::GetInstance()->Hero_attack_1_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_attack_1_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_attack_1_left)
	{
		CHero::GetInstance()->Hero_attack_1_left->Update(dt);

		if(CHero::GetInstance()->Hero_attack_1_left->m_anim->animeCurrentTime >= 0.69f)
		{
			//Response happens after a certain delay
			if(Sound_swordswing_delay >= 0.2f)
			{
				AttackResponse(CHero::ATTACK_1);
			}
		}
	}
	//===================================//
	//	Attack 2						//
	//=================================//

	CHero::GetInstance()->Hero_attack_2_right->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_attack_2_right->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_attack_2_right)
	{
		CHero::GetInstance()->Hero_attack_2_right->Update(dt);
		if(CHero::GetInstance()->Hero_attack_2_right->m_anim->animeCurrentTime >= 0.5f) 
		{
			//Response happens after a certain delay
			if(Sound_swordswing_delay >= 0.2f)
			{
				AttackResponse(CHero::ATTACK_2);
			}
		}
	}

	CHero::GetInstance()->Hero_attack_2_left->m_anim->animPosition.x = CHero::GetInstance()->GetHeroPos_x();	//Setting sprite position to hero position
	CHero::GetInstance()->Hero_attack_2_left->m_anim->animPosition.y =  CHero::GetInstance()->GetHeroPos_y();	//Setting sprite position to hero position
	if(CHero::GetInstance()->Hero_attack_2_left)
	{
		CHero::GetInstance()->Hero_attack_2_left->Update(dt);
		if(CHero::GetInstance()->Hero_attack_2_left->m_anim->animeCurrentTime >= 0.69f)
		{
			//Response happens after a certain delay
			if(Sound_swordswing_delay >= 0.1f)
			{
				AttackResponse(CHero::ATTACK_2);
			}
		}
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
void StudioProject::UpdateInput(double dt)
{
	//==================================//
	//			DYNAMIC CASTING 		//
	//==================================//
	CHero::GetInstance()->Hero_attack_1_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_ATTACK_1_RIGHT]);
	CHero::GetInstance()->Hero_run_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_RUN_RIGHT]);
	CHero::GetInstance()->Hero_run_left = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_RUN_LEFT]);
	CHero::GetInstance()->Hero_shockwave_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_SHOCKWAVE_RIGHT]);
	CHero::GetInstance()->Hero_jump_right = dynamic_cast<CSpriteAnimation*>(meshList[GEO_SABER_JUMP_RIGHT]);
	
	float tempHeroPos_x = CHero::GetInstance()->GetHeroPos_x();	//Hero current position X
	float tempHeroPos_y = CHero::GetInstance()->GetHeroPos_y();	//Hero current position Y
	int checkPosition_X = (int)((CHero::GetInstance()->GetMapOffset_x() + tempHeroPos_x) / m_cMap->GetTileSize());	//Hero tile position X
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (tempHeroPos_y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());	//Hero tile position Y
	/*if(Application::IsKeyPressed('B'))
	{
		m_cMap = m_cMap2;
	}*/

	//Movement
	if(Application::IsKeyPressed('W'))	//Move up
	{
		if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_CAVE)
		{
			Transiting = true;
			MapTransition->m_anim->animActive = true;
		}
	}
	//Down
	if(Application::IsKeyPressed('S'))
	{
		//this->CHero::GetInstance()->HeroMoveUpDown(false , 1.0f);
	}
	//Walk Left
	if(Application::IsKeyPressed('A') && 
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
	if(Application::IsKeyPressed('D') && 
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

	//JUMPING
	jump_input_delay += (float)dt;
	if(Application::IsKeyPressed(' ') && jump_input_delay > 0.3f)
	{
		CHero::GetInstance()->Getjumpcount()++;
		jump_input_delay = 0.f;
		CHero::GetInstance()->HeroJump();
	}

	//Attacks (checks if any other actions are being performed)
	Sound_swordswing_delay += (float)dt;

	if(Application::IsKeyPressed('X') && 
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == false)
	{
		if(CHero::GetInstance()->Gethero_face_right() == true)
		{
			CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive = true;
			CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive = false;
		}
		else if(CHero::GetInstance()->Gethero_face_left() == true)
		{
			CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive = false;
			CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive = true;
		}
	}
	else
	{
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive = false;
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animCurrentFrame = 0;
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animeCurrentTime = 0.f;

		CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive = false;
		CHero::GetInstance()->Hero_attack_1_left->m_anim->animCurrentFrame = 0;
		CHero::GetInstance()->Hero_attack_1_left->m_anim->animeCurrentTime = 0.f;
	}

	if(Application::IsKeyPressed('Z') && 
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == false)
	{
		if(CHero::GetInstance()->Gethero_face_right() == true)
		{
			CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive = true;
			CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive = false;
		}
		else if(CHero::GetInstance()->Gethero_face_left() == true)
		{
			CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive = false;
			CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive = true;
		}
	}
	else
	{
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive = false;
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animCurrentFrame = 0;
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animeCurrentTime = 0.f;

		CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive = false;
		CHero::GetInstance()->Hero_attack_2_left->m_anim->animCurrentFrame = 0;
		CHero::GetInstance()->Hero_attack_2_left->m_anim->animeCurrentTime = 0.f;
	}

	Sound_shockwave_delay += (float)dt;
	//Shockwave (checks if any other actions are being performed)
	if(Application::IsKeyPressed('C') && 
		CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == false && CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == false &&
		CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == false && CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == false &&
		CHero::GetInstance()->Gethero_EP() >= 10)
	{
		if(CHero::GetInstance()->Gethero_face_right() == true)
		{
			CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive = false;
			CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive = true;
		}
		else if(CHero::GetInstance()->Gethero_face_left() == true)
		{
			CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive = true;
			CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive = false;
		}
		CHero::GetInstance()->Gethero_EP() -= 10;
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
	if(MapTransition->m_anim->animCurrentFrame == 5 && Loaded == false)
	{
		LoadEnemies(m_CurrentLevel);
	}

	for(unsigned i = 0; i < enemyContainer.size(); ++i)
	{
		if(enemyContainer[i]->getActive() == true)
		{
			if(m_CurrentLevel == 1)
			{
				Lv1Clear = false;
			}
		}
		else if( i == (enemyContainer.size() - 1) )
		{
			if(m_CurrentLevel == 1)
			{
				Lv1Clear = true;
			}
		}
	}

	if(Lv1Clear)
	{
		GameMenu.setWinState(true);
		Reset(true);
	}
}
void StudioProject::Update(double dt)
{
	if(GameMenu.getReset() == true)
	{ 
		Reset(false);
	}
	if(GameMenu.getMenuState() == true || GameMenu.getLostState() == true || GameMenu.getWinState() == true)
	{	
		GameMenu.Update(dt);

		if( (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed(VK_RETURN)) && GameMenu.inputDelay == 0.f)
		{
			soundplayer.playSounds(soundplayer.MENU_FEEDBACK);
		}
	}
	else if(GameMenu.getMenuState() == false && GameMenu.getLostState() == false && GameMenu.getWinState() == false)
	{
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

		UpdateSprites(dt);
		UpdateEnemySprites(dt);

		soundplayer.playSounds(soundplayer.GAME_BGM);

		//camera.Update(dt);

		fps = (float)(1.f / dt);
	}
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
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			0);
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
void StudioProject::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
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
				Render2DMesh(meshList[GEO_PLAY_TEXT],false,1.f,130,350);
				Render2DMesh(meshList[GEO_CONTROLS_TEXT],false,1.f,80,220);
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
	case CMenuClass::LOSE_SCREEN:
		{
			if(GameMenu.getLostState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Game Over!", Color(1, 0, 0), 2.2f, 34, 54);

				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to return to main menu", Color(0.3f, 0.3f, 0.3f), 1.5f, 28, 10);
			}
		}
		break;
	case CMenuClass::WIN_SCREEN:
		{
			if(GameMenu.getWinState())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "You have cleared the Game!", Color(1, 0, 0), 2.2f, 28, 35);

				RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to return to main menu", Color(0.3f, 0.3f, 0.3f), 2.f, 28, 10);
			}
		}
		break;
	}
}
void StudioProject::RenderHUD(void)
{ 
	//ICON
	RenderMeshIn2D(meshList[GEO_ICON],false,0.7f,0.7f,-69.f,-51.f,false);

	//HUD template
	RenderMeshIn2D(meshList[GEO_HUD_TEMPLATE],false,2.f,2.f,-40.f,-51.f,false);
	//Health points
	RenderMeshIn2D(meshList[GEO_HUD_HP],false,0.032f * CHero::GetInstance()->Gethero_HP(),2.f,-52.8f,-48.1f,false);
	//Energy points
	RenderMeshIn2D(meshList[GEO_HUD_EP],false,0.032f * CHero::GetInstance()->Gethero_EP(),2.f,-52.8f,-53.7f,false);
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

	Render2DMesh(meshList[GEO_LAYER_2], false, 1.0f,(float)(100 - rearWallOffset_x));
	Render2DMesh(meshList[GEO_LAYER_2], false, 1.0f,(float)(1000 - rearWallOffset_x));

	rearWallOffset_x = (int) (CHero::GetInstance()->GetMapOffset_x() * 0.25);
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

	Render2DMesh(meshList[GEO_LAYER_3], false, 1.0f,(float)(100 - rearWallOffset_x));
	Render2DMesh(meshList[GEO_LAYER_3], false, 1.0f,(float)(1000 - rearWallOffset_x));
}
void StudioProject::RenderGoodies(void)
{
	/*for(unsigned i = 0; i< 10; ++i)
	{
		Render2DMesh(theArrayOfGoodies[i]->GetMesh(),false,1.0f,(float)theArrayOfGoodies[i]->GetPos_x(),(float)theArrayOfGoodies[i]->GetPos_y());
	}*/
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
			if (m_cMap->theScreenMap[i][m] == TILE_GRASS)
			{
				Render2DMesh(meshList[GEO_TILEGRASSGROUND], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == TILE_UNDERGROUND)
			{
				Render2DMesh(meshList[GEO_TILEUNDERGROUND], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == TILE_CAVE)
			{
				Render2DMesh(meshList[GEO_TILECAVE], false, 1.f, (float)k*m_cMap->GetTileSize()-CHero::GetInstance()->GetMapFineOffset_x(), 575.f-i*m_cMap->GetTileSize());
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
			if (m_cRearMap->theScreenMap[i][m] == 3)
			{
				Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1.0f, (float)k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, 575.f-i*m_cRearMap->GetTileSize());
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

	/*std::ostringstream sss;q
	sss.precision(5);
	sss << "heropos.x: "<< CHero::GetInstance()->GetMapOffset_x() + CHero::GetInstance()->GetHeroPos_x();
	RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 3, 2, 7);

	std::ostringstream ssss;
	ssss.precision(5);
	ssss << "heropos.x: "<<CHero::GetInstance()->GetHeroPos_y();
	RenderTextOnScreen(meshList[GEO_TEXT], ssss.str(), Color(0, 1, 0), 3, 2, 4);*/
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
		Render2DSprite(CHero::GetInstance()->Hero_idle_right,false,CHero::GetInstance()->Hero_idle_right->m_anim->animScale,CHero::GetInstance()->Hero_idle_right->m_anim->animScale,CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_idle_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	if(CHero::GetInstance()->Hero_idle_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_idle_left,false,CHero::GetInstance()->Hero_idle_left->m_anim->animScale,CHero::GetInstance()->Hero_idle_left->m_anim->animScale,CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_idle_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	//===================================================================================//
	//				
	//							RUN													//
	//
	//==================================================================================//
	if(CHero::GetInstance()->Hero_run_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_run_right,false,CHero::GetInstance()->Hero_run_right->m_anim->animScale,CHero::GetInstance()->Hero_run_right->m_anim->animScale,CHero::GetInstance()->Hero_run_right->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_run_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	if(CHero::GetInstance()->Hero_run_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_run_left,false,CHero::GetInstance()->Hero_run_left->m_anim->animScale,CHero::GetInstance()->Hero_run_left->m_anim->animScale,CHero::GetInstance()->Hero_run_left->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_run_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}

	//===================================================================================//
	//				
	//							ATTACKS													//
	//
	//==================================================================================//
	//Attack 1
	if(CHero::GetInstance()->Hero_attack_1_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_attack_1_right,false,CHero::GetInstance()->Hero_attack_1_right->m_anim->animScale,CHero::GetInstance()->Hero_attack_1_right->m_anim->animScale,CHero::GetInstance()->Hero_attack_1_right->m_anim->animPosition.x + HERO_OFFSET + 16.5f,CHero::GetInstance()->Hero_attack_1_right->m_anim->animPosition.y + HERO_OFFSET + 16.5f,false);
	}

	if(CHero::GetInstance()->Hero_attack_1_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_attack_1_left,false,CHero::GetInstance()->Hero_attack_1_left->m_anim->animScale,CHero::GetInstance()->Hero_attack_1_left->m_anim->animScale,CHero::GetInstance()->Hero_attack_1_left->m_anim->animPosition.x + HERO_OFFSET + 16.5f,CHero::GetInstance()->Hero_attack_1_left->m_anim->animPosition.y + HERO_OFFSET + 16.5f,false);
	}

	//Attack 2
	if(CHero::GetInstance()->Hero_attack_2_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_attack_2_right,false,CHero::GetInstance()->Hero_attack_2_right->m_anim->animScale,CHero::GetInstance()->Hero_attack_2_right->m_anim->animScale,CHero::GetInstance()->Hero_attack_2_right->m_anim->animPosition.x + HERO_OFFSET + 4.125f,CHero::GetInstance()->Hero_attack_2_right->m_anim->animPosition.y + HERO_OFFSET + 4.125f,false);
	}
	if(CHero::GetInstance()->Hero_attack_2_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_attack_2_left,false,CHero::GetInstance()->Hero_attack_2_left->m_anim->animScale,CHero::GetInstance()->Hero_attack_2_left->m_anim->animScale,CHero::GetInstance()->Hero_attack_2_left->m_anim->animPosition.x + HERO_OFFSET + 4.125f,CHero::GetInstance()->Hero_attack_2_left->m_anim->animPosition.y + HERO_OFFSET + 4.125f,false);
	}

	//Shockwave
	if(CHero::GetInstance()->Hero_shockwave_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_shockwave_right,false,CHero::GetInstance()->Hero_shockwave_right->m_anim->animScale,CHero::GetInstance()->Hero_shockwave_right->m_anim->animScale,CHero::GetInstance()->Hero_shockwave_right->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_shockwave_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	if(CHero::GetInstance()->Hero_shockwave_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_shockwave_left,false,CHero::GetInstance()->Hero_shockwave_left->m_anim->animScale,CHero::GetInstance()->Hero_shockwave_left->m_anim->animScale,CHero::GetInstance()->Hero_shockwave_left->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_shockwave_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	//===================================================================================//
	//				
	//							JUMP													//
	//
	//==================================================================================//
	if(CHero::GetInstance()->Hero_jump_right->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_jump_right,false,CHero::GetInstance()->Hero_jump_right->m_anim->animScale,CHero::GetInstance()->Hero_jump_right->m_anim->animScale,CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_jump_right->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	if(CHero::GetInstance()->Hero_jump_left->m_anim->animActive == true)
	{
		Render2DSprite(CHero::GetInstance()->Hero_jump_left,false,CHero::GetInstance()->Hero_jump_left->m_anim->animScale,CHero::GetInstance()->Hero_jump_left->m_anim->animScale,CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.x + HERO_OFFSET,CHero::GetInstance()->Hero_jump_left->m_anim->animPosition.y + HERO_OFFSET,false);
	}
	
}
void StudioProject::RenderEnemySprites(CEnemy* enemyInput)
{
	//Walking right
	if(enemyInput->WalkAnimation_Right->m_anim->animActive == true)
	{
		int enemy_x = enemyInput->GetPos_x() - CHero::GetInstance()->GetMapOffset_x();
		int enemy_y = enemyInput->GetPos_y();

		Render2DSprite(enemyInput->WalkAnimation_Right,false,enemyInput->WalkAnimation_Right->m_anim->animScale,enemyInput->WalkAnimation_Right->m_anim->animScale,
			enemy_x + ENEMY_OFFSET,enemyInput->GetPos_y() + ENEMY_OFFSET,false);

	}
	//Walkng left
	if(enemyInput->WalkAnimation_Left->m_anim->animActive == true)
	{
		int enemy_x = enemyInput->GetPos_x() - CHero::GetInstance()->GetMapOffset_x();
		int enemy_y = enemyInput->GetPos_y();

		Render2DSprite(enemyInput->WalkAnimation_Left,false,enemyInput->WalkAnimation_Left->m_anim->animScale,enemyInput->WalkAnimation_Left->m_anim->animScale,
			enemy_x + ENEMY_OFFSET,enemyInput->GetPos_y() + ENEMY_OFFSET,false);

	}

	//death animation
	if(enemyInput->DeathAnimation_Right->m_anim->animActive == true)
	{
		int enemy_x = enemyInput->GetPos_x() - CHero::GetInstance()->GetMapOffset_x();
		int enemy_y = enemyInput->GetPos_y();

		Render2DSprite(enemyInput->DeathAnimation_Right,false,enemyInput->DeathAnimation_Right->m_anim->animScale,enemyInput->DeathAnimation_Right->m_anim->animScale,
			(enemyInput->DeathAnimation_Right->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->DeathAnimation_Right->m_anim->animPosition.y + ENEMY_OFFSET,false);

	}

	//Attack Right
	if(enemyInput->AttackAnimation_Right->m_anim->animActive == true)
	{
		Render2DSprite(enemyInput->AttackAnimation_Right,false,enemyInput->AttackAnimation_Right->m_anim->animScale,enemyInput->AttackAnimation_Right->m_anim->animScale,
			(enemyInput->AttackAnimation_Right->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->AttackAnimation_Right->m_anim->animPosition.y + ENEMY_OFFSET + 13.5f,false);
	}

	//Attack Left
	if(enemyInput->AttackAnimation_Left->m_anim->animActive == true)
	{

		Render2DSprite(enemyInput->AttackAnimation_Left,false,enemyInput->AttackAnimation_Left->m_anim->animScale,enemyInput->AttackAnimation_Left->m_anim->animScale,
			(enemyInput->AttackAnimation_Left->m_anim->animPosition.x - CHero::GetInstance()->GetMapOffset_x()) + ENEMY_OFFSET,enemyInput->AttackAnimation_Left->m_anim->animPosition.y + ENEMY_OFFSET + 13.5f,false);
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

	if(GameMenu.getMenuState() == true && GameMenu.getControlState() == false  && GameMenu.getLostState() == false)	//Rendering Default screen
	{
		RenderMenu(CMenuClass::PLAY_GAME);
	}
	else if(GameMenu.getMenuState() == true && GameMenu.getControlState() == true && GameMenu.getLostState() == false)	//Rendering Control Screen
	{
		RenderMenu(CMenuClass::CONTROLS);
	}
	else if(GameMenu.getMenuState() == false && GameMenu.getLostState() == false && GameMenu.getWinState() == false)
	{
		/*===================
			GAME START
		=====================*/
		RenderBackground();

		//RenderRearTileMap();

		RenderTileMap();
		RenderGoodies();
		for(std::vector<CEnemy *>::iterator it = enemyContainer.begin(); it != enemyContainer.end(); ++it)
		{
			CEnemy *Enemy = (CEnemy *)*it;

			if(Enemy->getActive())
			{
				RenderEnemySprites(Enemy);
			}
		}

		RenderHeroSprites();

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
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}