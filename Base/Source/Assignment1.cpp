#include "Assignment1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include <ostream>
#include <fstream>

Assignment1::Assignment1()
	:m_cMinimap(NULL),
	object(NULL),
	enemy(NULL),
	engine(NULL),
	Sound(NULL)
{

}
Assignment1::~Assignment1()
{
}

void Assignment1::InitGL()
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
void Assignment1::InitShaders()
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

	//Handle for fog
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogParam.enabled");
	
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

	fog.color.Set(0.5f,0.5f,0.5f);
	fog.start = 100.f;
	fog.end = 1000.f;
	fog.density = 0.005f;
	fog.type = 0;
	fog.enabled = true;
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fog.color.r);
	glUniform1f(m_parameters[U_FOG_START], fog.start);
	glUniform1f(m_parameters[U_FOG_END], fog.end);
	glUniform1f(m_parameters[U_FOG_DENSITY] , fog.density);
	glUniform1i(m_parameters[U_FOG_TYPE], fog.type);
	glUniform1i(m_parameters[U_FOG_ENABLE], fog.enabled);
}
void Assignment1::InitMesh()
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
	meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//box.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_OBJECT]->textureArray[0] = LoadTGA("Image//chair.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	//Bullet
	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Bullet", Color(0.737f, 0.737f, 0.737f), 18, 36, 1.f);
	
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureArray[0] = LoadTGA("Image//floor1.tga");
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 1.f);
	meshList[GEO_FLOOR]->textureArray[0] = LoadTGA("Image//floor1.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("Table", "OBJ//Table.obj");
	meshList[GEO_TABLE]->textureArray[0] = LoadTGA("Image//Table.tga");

	//Minimap
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("MiniMap",Color(1,1,1),1.f));
	m_cMinimap->GetBackground()->textureID = LoadTGA("Image//MinimapBackground.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateMinimapBorder("MinimapBorder",Color(0,0,0),1.f));
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("MinimapAvatar",Color(0,0,0),1.f));

	//Enemy on minimap
	meshList[GEO_MAP_ENEMY] = MeshBuilder::GenerateQuad("MapEnemy", Color(1, 1, 1), 1.f);
	meshList[GEO_MAP_ENEMY]->textureID = LoadTGA("Image//MapEnemy.tga");

	//Skyplane
	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 128, 200.0f, 2000.0f, 1.0f, 1.0f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//skytop.tga");

	//Terrain
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN",  "Image//testmap3.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//SnowFloor.tga");

	//Tree
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//CartoonTree.obj");
	meshList[GEO_TREE]->textureArray[0] = LoadTGA("Image//treetexture.tga");
	
	Vector3 Pos;
	float offsetX;

	//Fence
	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("Fence", "OBJ//Fence.obj");
	meshList[GEO_FENCE]->textureArray[0] = LoadTGA("Image//FenceText.tga");

	meshList[GEO_AMMOBOX] = MeshBuilder::GenerateOBJ("AmmoBox", "OBJ//AmmoBox.obj");
	meshList[GEO_AMMOBOX]->textureArray[0] = LoadTGA("Image//AmmoBox.tga");
	Pos.Set(0.f,0.f,0.f);
	for(unsigned i = 0; i < 3; ++i)
	{
		object = new C3DObject;
		object->SetData(meshList[GEO_AMMOBOX],Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f) + Pos.y,Pos.z),Vector3(5.f,5.f,5.f),false,false);
		object->getBoundBox().CreateBoundingBox(Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f)+ Pos.y,Pos.z),3.605f * 5.f,3.605f* 5.f,3.605f* 5.f);
		AmmoBoxes.push_back(object);
	}

	offsetX = -375.f;
	for(unsigned i = 0;i < 10; ++i)
	{
		Pos.Set(offsetX,-6.f,-95.f);
		object = new C3DObject;
		object->SetData(meshList[GEO_FENCE],Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f) + Pos.y,Pos.z),Vector3(1.f,1.f,1.f),true,false);
		object->getBoundBox().CreateBoundingBox(Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f)+ Pos.y,Pos.z),159.f,159.f,35.8f);
		staticObjects.push_back(object);
		offsetX -= 150.f;
	}
	offsetX = 370.f;
	for(unsigned i = 0;i < 9; ++i)
	{
		Pos.Set(offsetX,-6.f,-95.f);
		object = new C3DObject;
		object->SetData(meshList[GEO_FENCE],Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f) + Pos.y,Pos.z),Vector3(1.f,1.f,1.f),true,false);
		object->getBoundBox().CreateBoundingBox(Vector3(Pos.x,(getHeight(m_heightMap,Pos.x / 4000.f,Pos.z/4000.f) * 350.f)+ Pos.y,Pos.z),159.f,159.f,35.8f);
		staticObjects.push_back(object);
		offsetX += 150.f;
	}

	for(unsigned i = 0;i < staticObjects.size();++i)
	{
		camera.hitbox.push_back(staticObjects[i]->getBoundBox());
	}

	player.getBoundBox().CreateBoundingBox(camera.position,20.f,20.f,20.f);
}
void Assignment1::InitEnemy()
{
	//Enemy
	Vector3 Pos;
	meshList[GEO_SNOWMAN] = MeshBuilder::GenerateOBJ("SnowMan", "OBJ//SnowManbox.obj");
	meshList[GEO_SNOWMAN]->textureArray[0] = LoadTGA("Image//SnowManTexture.tga");
	for(unsigned i = 0;i < 10;++i)
	{
		enemy = new CEnemy;
		enemy->SetData(meshList[GEO_SNOWMAN],Vector3(1433.f,(getHeight(m_heightMap,0.0f / 4000.f,0.0f/4000.f) * 350.f),633.f),Vector3(6.5f,6.5f,6.5f),false,false);
		enemy->getBoundBox().CreateBoundingBox(Vector3(1433.f,(getHeight(m_heightMap,0.0f / 4000.f,0.0f/4000.f) * 350.f),633.f),7.f * 6.5f,6.f * 6.5f,4.f * 6.5f);
		enemy->setHitpoints(100);
		enemy->setSpeed(250.f);
		enemy->seteTarget(Vector3(1433.f,(getHeight(m_heightMap,0.0f / 4000.f,0.0f/4000.f) * 350.f),632.f));
		enemyContainer.push_back(enemy);
	}

	for(unsigned i = 0;i < enemyContainer.size();++i)
	{
		camera.hitbox.push_back(enemyContainer[i]->getBoundBox());
	}

}
void Assignment1::InitGun()
{
	//DEAGLE
	meshList[GEO_DEAGLE] = MeshBuilder::GenerateOBJ("Deagle", "OBJ//deagle.obj");
	meshList[GEO_DEAGLE]->textureArray[0] = LoadTGA("Image//pist_deagle2.tga");

	//M4A4
	meshList[GEO_M4A4] = MeshBuilder::GenerateOBJ("M4A4", "OBJ//m4a4.obj");
	meshList[GEO_M4A4]->textureArray[0] = LoadTGA("Image//m4a4_rifle.tga");
	Gun = new CWeapon;
	Gun->SetData(meshList[GEO_M4A4],GEO_M4A4,0.05f,90,30,30,1.1f,camera.position,Vector3(-3.5f,-5.5f,5.5f));
	inventory.push_back(Gun);
	Gun = new CWeapon;
	Gun->SetData(meshList[GEO_DEAGLE],GEO_DEAGLE,0.1f,50,15,15,1.0f,camera.position,Vector3(-2.5f,5.5f,-2.5f));
	inventory.push_back(Gun);
	player.setWeapon(inventory.at(1));
}
void Assignment1::InitCamera()
{
	camera.Init(Vector3(-1000, 68, 10), Vector3(-1000, 68, 0), Vector3(0, 1, 0));
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void Assignment1::InitVariables()
{
	rotateAngle = 0;
	firerate = 0;

	NumTree = 27;
	for(unsigned i = 0; i < NumTree; ++i)
	{
		TreeoffsetZ.push_back(Math::RandFloatMinMax(-900,-1100));
	}

	bLightEnabled = true;

	isBack = true;

	enemySpawn = 0.f;
	ENEMY_RATE = 1.2f;

	ammoSpawn = 0.f;
	AMMO_RATE = 10.f;

	score = 0;

	//Sound Engine
	engine = createIrrKlangDevice();
	if (!engine)
	{
		printf("Could not startup engine\n");
	}

}
void Assignment1::InitHUD()
{
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair",Color(1, 1, 1), 20.f);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair1.tga");

	meshList[GEO_HEALTHHUD] = MeshBuilder::GenerateQuad("healthHUD",Color(1, 1, 1), 20.f);
	meshList[GEO_HEALTHHUD]->textureID = LoadTGA("Image//healthHUD2.tga");
	meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("healthbar",Color(1, 1, 1), 20.f);
	meshList[GEO_HEALTHBAR]->textureID = LoadTGA("Image//healthBar2.tga");
}

void Assignment1::Init()
{
	InitGL();
	InitShaders();

	InitCamera();

	InitMesh();
	InitEnemy();
	InitGun();
	InitHUD();

	InitVariables();
}

void Assignment1::UpdateAmmoBox(double dt)
{
	ammoSpawn += (float)dt;
	float tempX = Math::RandFloatMinMax(-1000.f,1000.f);
	float tempZ = Math::RandFloatMinMax(0.f,1300.f);
	if(ammoSpawn > AMMO_RATE)
	{
		for(unsigned i = 0; i < AmmoBoxes.size(); ++i)
		{
			if(AmmoBoxes[i]->getRender() == false)
			{
				AmmoBoxes[i]->setRender(true);
				AmmoBoxes[i]->setObjPos(Vector3(tempX,(getHeight(m_heightMap,tempX / 4000.f,tempZ/4000.f) * 350.f) + 6.f,tempZ));
				ammoSpawn = 0.f;
				break;
			}
		}
	}

	for(unsigned i = 0;i < AmmoBoxes.size();++i)
	{
		if((AmmoBoxes[i]->getPos() - camera.position).Length() <= 30 && AmmoBoxes[i]->getRender() == true)
		{
			if(player.getWeapon()->getIndex() == GEO_DEAGLE && player.getWeapon()->getAmmoCount() < 50)
			{
				if(player.getWeapon()->getAmmoCount() > 40)
				{
					player.getWeapon()->setAmmoCount(50);
				}
				else
				{
					player.getWeapon()->setAmmoCount(player.getWeapon()->getAmmoCount() + 10);
				}
			}
			else if(player.getWeapon()->getIndex() == GEO_M4A4 && player.getWeapon()->getAmmoCount() < 90)
			{
				if(player.getWeapon()->getAmmoCount() > 70)
				{
					player.getWeapon()->setAmmoCount(90);
				}
				else
				{
					player.getWeapon()->setAmmoCount(player.getWeapon()->getAmmoCount() + 20);
				}
			}
			AmmoBoxes[i]->setRender(false);
		}
	}
}
void Assignment1::UpdateGun(double dt)
{
	if(Application::IsKeyPressed('1'))
	{
		if(player.getWeapon()->getReloading() == false && isBack == true)
		{
			player.setWeapon(inventory.at(0));
		}
	}
	if(Application::IsKeyPressed('2'))
	{
		if(player.getWeapon()->getReloading() == false && isBack == true)
		{
			player.setWeapon(inventory.at(1));
		}
	}
}
void Assignment1::UpdateDebug(double dt)
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
void Assignment1::UpdateGunAnimation(double dt)
{
	//=====================================================================================//
	//																						//
	//								FIRE ANIMATION											//
	//																						//
	//=====================================================================================//
	//DEAGLE
	if(player.getWeapon()->getIndex() == GEO_DEAGLE)
	{
		if(player.getWeapon()->getOffsetZ() != -2.5f)
		{
			float offsetZ = player.getWeapon()->getOffsetZ();
			offsetZ	-= (float)dt * 10.f;
			player.getWeapon()->setOffsetZ(offsetZ);
			if(player.getWeapon()->getOffsetZ() <= -2.5f)
			{
				player.getWeapon()->setOffsetZ(-2.5f);
			}
		}
		/*if(player.getWeapon()->getGunOffset().z != -2.5f &&player.getWeapon()->getAimDown() == true )
		{
			float offsetZ = player.getWeapon()->getGunOffset().z;
			offsetZ	-= (float)dt * 10.f;
			player.getWeapon()->setOffSet(Vector3(-2.5f,5.5f,offsetZ));
			if(player.getWeapon()->getGunOffset().z <= -2.5f)
			{
				player.getWeapon()->setOffSet(Vector3(1.2f,5.5f,-2.5f));
			}
		}*/
	}
	//M4A4
	else if(player.getWeapon()->getIndex() == GEO_M4A4)
	{
		if(player.getWeapon()->getOffsetZ() != 5.5f)
		{
			float offsetZ = player.getWeapon()->getOffsetZ();
			offsetZ	+= (float)dt * 15.f;
			player.getWeapon()->setOffsetZ(offsetZ);
			if(player.getWeapon()->getOffsetZ() >= 5.5f)
			{
				player.getWeapon()->setOffsetZ(5.5f);
			}
		}
	}
	//=====================================================================================//
	//																						//
	//								RELOAD ANIMATION										//
	//																						//
	//=====================================================================================//

	//DEAGLE
	if(player.getWeapon()->getIndex() == GEO_DEAGLE && player.getWeapon()->getReloading() == false)
	{
		if(player.getWeapon()->getOffsetY() != 5.5f)
		{
			float offsetY = player.getWeapon()->getOffsetY();
			offsetY	-= (float)dt * 15.f;
			player.getWeapon()->setOffsetY(offsetY);
			if(player.getWeapon()->getOffsetY() <= 5.5f)
			{
				player.getWeapon()->setOffsetY(5.5f);
				isBack = true;
			}
		}
	}
	//M4A4
	else if(player.getWeapon()->getIndex() == GEO_M4A4  && player.getWeapon()->getReloading() == false)
	{
		if(player.getWeapon()->getOffsetY() != -5.5f)
		{
			float offsetY = player.getWeapon()->getOffsetY();
			offsetY	+= (float)dt * 15.f;
			player.getWeapon()->setOffsetY(offsetY);
			if(player.getWeapon()->getOffsetY() >= -5.5f)
			{
				player.getWeapon()->setOffsetY(-5.5f);
				isBack = true;
			}
		}
	}
	//=====================================================================================//
	//																						//
	//								AIMDOWN ANIMATION										//
	//																						//
	//=====================================================================================//
	//DEAGLE
	if(player.getWeapon()->getIndex() == GEO_DEAGLE && player.getWeapon()->getAimDown() == true && isBack == true)
	{
		float offsetX = player.getWeapon()->getOffsetX();
		float offsetY = player.getWeapon()->getOffsetY();
		offsetX	+= (float)dt * 40.f;
		offsetY -= (float)dt * 11.9f;
		player.getWeapon()->setOffsetX(offsetX);
		player.getWeapon()->setOffsetY(offsetY);
		if(player.getWeapon()->getOffsetX() >= 1.2f)
		{
			player.getWeapon()->setOffsetX(1.2f);
			player.getWeapon()->setOffsetY(4.5f);
		}
	}
	else if(player.getWeapon()->getIndex() == GEO_DEAGLE && 
		player.getWeapon()->getAimDown() == false && 
		player.getWeapon()->getOffsetX() != -2.5f)
	{
		float offsetX = player.getWeapon()->getOffsetX();
		float offsetY = player.getWeapon()->getOffsetY();
		offsetX	-= (float)dt * 40.f;
		offsetY += (float)dt * 11.9f;
		player.getWeapon()->setOffsetX(offsetX);
		player.getWeapon()->setOffsetY(offsetY);
		if(player.getWeapon()->getOffsetX() <= -2.5f)
		{
			player.getWeapon()->setOffsetX(-2.5f);
			player.getWeapon()->setOffsetY(5.5f);
		}
	}
	//M4A4
	if(player.getWeapon()->getIndex() == GEO_M4A4 && player.getWeapon()->getAimDown() == true && isBack == true)
	{
		float offsetX = player.getWeapon()->getOffsetX();
		float offsetY = player.getWeapon()->getOffsetY();
		offsetX	+= (float)dt * 40.f;
		//offsetY += (float)dt * 11.9f;
		player.getWeapon()->setOffsetX(offsetX);
		player.getWeapon()->setOffsetY(offsetY);
		if(player.getWeapon()->getOffsetX() >= -0.143f)
		{
			player.getWeapon()->setOffsetX(-0.143f);
			//player.getWeapon()->setOffsetY(4.5f);
		}
	}
	else if(player.getWeapon()->getIndex() == GEO_M4A4 && 
		player.getWeapon()->getAimDown() == false && 
		player.getWeapon()->getOffsetX() != -3.5f)
	{
		float offsetX = player.getWeapon()->getOffsetX();
		float offsetY = player.getWeapon()->getOffsetY();
		offsetX	-= (float)dt * 40.f;
		offsetY += (float)dt * 11.9f;
		player.getWeapon()->setOffsetX(offsetX);
		player.getWeapon()->setOffsetY(offsetY);
		if(player.getWeapon()->getOffsetX() <= -3.5f)
		{
			player.getWeapon()->setOffsetX(-3.5f);
			player.getWeapon()->setOffsetY(-5.5f);
		}
	}
}
void Assignment1::UpdateFire(double dt)
{
	//=====================================================================================//
	//																						//
	//								RELOADING												//
	//																						//
	//=====================================================================================//
	if(Application::IsKeyPressed('R') && player.getWeapon()->getCurrentAmmo() < player.getWeapon()->getMaxAmmo() 
		&& player.getWeapon()->getAmmoCount() != 0 
		&& player.getWeapon()->getReloading() == false)
	{
		player.getWeapon()->setReloading(true);
		if(player.getWeapon()->getIndex() == GEO_DEAGLE)
		{
			Sound = engine->play2D("Sound\\DEagle_Reload2.wav");
			player.getWeapon()->setOffSet(Vector3(-2.5f,5.5f,-2.5f));
			player.getWeapon()->setOffsetY(8.0f);
			isBack = false;
		}
		else if(player.getWeapon()->getIndex() == GEO_M4A4)
		{
			Sound = engine->play2D("Sound\\M4A4_Reload.wav");
			player.getWeapon()->setOffSet(Vector3(-3.5f,-5.5f,5.5f));
			player.getWeapon()->setOffsetY(-8.0f);
			isBack = false;
		}
	}
	firerate += (float)dt;	//Firerate 

	//=====================================================================================//
	//																						//
	//								DESERT EAGLE											//
	//																						//
	//=====================================================================================//
	
	if(Application::IsMousePressed(0) && player.getWeapon()->getIndex() == GEO_DEAGLE && player.getWeapon()->getCurrentAmmo() > 0
		&& player.getWeapon()->getReloading() == false
		&& isBack == true)	//If gun is back at start position after reload
	{
		if(firerate > player.getWeapon()->getRECOIL_RATE() && player.getWeapon()->getAimDown() == false)
		{
			Sound = engine->play2D("Sound\\DEagle_Fire2.wav");
			player.getWeapon()->Shoot();
			CBullet PistolAmmo;
			PistolAmmo.SetData(meshList[GEO_BULLET],camera.position,10.0f,true);
			Vector3 direction = camera.target - camera.position;
			PistolAmmo.SetDirection(direction);
			BulletContainer.push_back(PistolAmmo);
			player.getWeapon()->setOffsetZ(0.f);
			firerate = 0.f;
			camera.Gunrecoil = true;
		}
		else if(firerate > player.getWeapon()->getRECOIL_RATE() && player.getWeapon()->getAimDown() == true)
		{
			Sound = engine->play2D("Sound\\DEagle_Fire2.wav");
			player.getWeapon()->Shoot();
			CBullet PistolAmmo;
			PistolAmmo.SetData(meshList[GEO_BULLET],camera.position,10.0f,true);
			Vector3 direction = camera.target - camera.position;
			PistolAmmo.SetDirection(direction);
			BulletContainer.push_back(PistolAmmo);
			player.getWeapon()->setOffsetZ(0.f);
			firerate = 0.f;
		}
	}
	//Aiming down sight
	else if(Application::IsMousePressed(1) && player.getWeapon()->getIndex() == GEO_DEAGLE)
	{
		player.getWeapon()->setAimDown(true);
		camera.CAMERA_SPEED = 100.f;
	}
	//=====================================================================================//
	//																						//
	//										M4A4											//
	//																						//
	//=====================================================================================//

	else if(Application::IsMousePressed(0) && player.getWeapon()->getIndex() == GEO_M4A4 && player.getWeapon()->getCurrentAmmo() > 0
				&& player.getWeapon()->getReloading() == false
				&& isBack == true)	//If gun is back at start position after reload
	{
		if(firerate > player.getWeapon()->getRECOIL_RATE()  && player.getWeapon()->getAimDown() == false)
		{
			Sound = engine->play2D("Sound\\M4A4_Fire.wav");
			player.getWeapon()->Shoot();
			CBullet RifleAmmo;
			RifleAmmo.SetData(meshList[GEO_BULLET],camera.position,10.0f,true);
			Vector3 direction = camera.target - camera.position;
			RifleAmmo.SetDirection(direction);
			BulletContainer.push_back(RifleAmmo);
			player.getWeapon()->setOffsetZ(3.f);
			firerate = 0.f;
			camera.Gunrecoil = true;
		}
		else if(firerate > player.getWeapon()->getRECOIL_RATE()  && player.getWeapon()->getAimDown() == true)
		{
			Sound = engine->play2D("Sound\\M4A4_Fire.wav");
			player.getWeapon()->Shoot();
			CBullet RifleAmmo;
			RifleAmmo.SetData(meshList[GEO_BULLET],camera.position,10.0f,true);
			Vector3 direction = camera.target - camera.position;
			RifleAmmo.SetDirection(direction);
			BulletContainer.push_back(RifleAmmo);
			player.getWeapon()->setOffsetZ(4.5f);
			firerate = 0.f;
		}
	}
	//Aim down sight
	else if(Application::IsMousePressed(1) && player.getWeapon()->getIndex() == GEO_M4A4)
	{
		player.getWeapon()->setAimDown(true);
		camera.CAMERA_SPEED = 100.f;
	}
	else
	{
		player.getWeapon()->setAimDown(false);
		camera.Gunrecoil = false;
		camera.CAMERA_SPEED = 200.f;
	}
	

	UpdateGunAnimation(dt);

	player.getWeapon()->UpdateReload(dt);
}
void Assignment1::UpdateBullet(double dt)
{
	if(BulletContainer.size() > 0)
	{
		for(unsigned i = 0;i < BulletContainer.size(); ++i)
		{
			if(BulletContainer[i].GetStatus() == true)
			{
				BulletContainer[i].Update();
				//If collides with terrain
				if(BulletContainer[i].getPos().y <= (getHeight(m_heightMap,BulletContainer[i].getPos().x / 4000.f,BulletContainer[i].getPos().z/ 4000.f) * 350.f))
				{
					BulletContainer[i].SetStatus(false);
				}
				//If out of bounds
				if(BulletContainer[i].getPos().x <= -2000.f 
					|| BulletContainer[i].getPos().x >= 2000.f 
					|| BulletContainer[i].getPos().y > 2000.f 
					|| BulletContainer[i].getPos().z >= 2000.f 
					|| BulletContainer[i].getPos().z <= -2000.f)
				{
					BulletContainer[i].SetStatus(false);
				}
				//if hit enemy
				for(unsigned j = 0; j < enemyContainer.size(); ++j)
				{
					if(enemyContainer[j]->getRender() == true)
					{
						if((BulletContainer[i].getPos() - enemyContainer[j]->getPos()).Length() < 30)
						{
							BulletContainer[i].SetStatus(false);
							enemyContainer[j]->setHitpoints(enemyContainer[j]->getHitpoints() - 40);
						}
					}
				}
			}
		}
	}
}
void Assignment1::CameraUpdate(double dt)
{			
	camera.Update(dt);
}
void Assignment1::UpdateCameraStatus(const unsigned char key)
{
	camera.UpdateStatus(key);
}
void Assignment1::UpdateCameraFalseStatus(const unsigned char key)
{
	camera.UpdateFalseStatus(key);
}
void Assignment1::UpdateCollision(double dt)
{
	//Getting height of terrain at player pos
	camera.terrainHeight = getHeight(m_heightMap,camera.position.x / 4000.f,camera.position.z/ 4000.f) * 350.f;

	player.setObjPos(camera.position);
	//player.rotate.SetToRotation(camera.cameraRotate.y,0,1,0);
	player.UpdateBoundBox();

	for(unsigned i = 0;i < enemyContainer.size();++i)
	{
		enemyContainer[i]->UpdateBoundBox();
	}
}
void Assignment1::UpdateEnemy(double dt)
{
	enemySpawn += (float)dt;
	int temp = Math::RandIntMinMax(1,3);
	if(enemySpawn > ENEMY_RATE)
	{
		for(unsigned i = 0;i < enemyContainer.size();++i)
		{
			if(enemyContainer[i]->getRender() == false)
			{
				enemyContainer[i]->setRender(true);
				enemyContainer[i]->setHitpoints(100);
				if(temp == 1)
				{
					enemyContainer[i]->setObjPos(Vector3(1433.f,(getHeight(m_heightMap,1433.f / 4000.f,633.f/4000.f) * 350.f),633.f));
				}
				else if(temp == 2)
				{
					enemyContainer[i]->setObjPos(Vector3(1433.f,(getHeight(m_heightMap,1433.f / 4000.f,933.f/4000.f) * 350.f),933.f));
				}
				else if(temp == 3)
				{
					enemyContainer[i]->setObjPos(Vector3(1433.f,(getHeight(m_heightMap,1433.f / 4000.f,333.f/4000.f) * 350.f),333.f));
				}
				enemySpawn = 0.f;
				break;
			}
		}
	}

	for(unsigned i = 0;i < enemyContainer.size();++i)
	{
		if(enemyContainer[i]->getRender() == true)
		{
			enemyContainer[i]->Update(dt,camera.position);
			enemyContainer[i]->setObjPos(Vector3(enemyContainer[i]->getPos().x,(getHeight(m_heightMap,enemyContainer[i]->getPos().x / 4000.f,enemyContainer[i]->getPos().z/4000.f) * 350.f),enemyContainer[i]->getPos().z));
			if((enemyContainer[i]->getPos() - camera.position).Length() <= 60)
			{
				player.setHitpoints(player.getHitpoints() - 10);
				enemyContainer[i]->setRender(false);
			}
			if(enemyContainer[i]->getHitpoints() <= 0)
			{
				enemyContainer[i]->setRender(false);
				score += 25;
			}
		}
	}

}
void Assignment1::UpdateRestart(double dt)
{
	if(player.getHitpoints() <= 0)
	{
		score  = 0;
		camera.position.Set(-1000, 68, 10);
		camera.target.Set(-1000, 68, 0);
		for(unsigned i = 0; i < enemyContainer.size(); ++i)
		{
			if(enemyContainer[i]->getRender() == true)
			{
				enemyContainer[i]->setRender(false);
			}
		}

		for(unsigned i = 0; i < AmmoBoxes.size(); ++i)
		{
			if(AmmoBoxes[i]->getRender() == true)
			{
				AmmoBoxes[i]->setRender(false);
			}
		}
		player.setHitpoints(100);
	
		inventory.at(0)->SetData(meshList[GEO_M4A4],GEO_M4A4,0.05f,90,30,30,1.1f,camera.position,Vector3(-3.5f,-5.5f,5.5f));
		inventory.at(1)->SetData(meshList[GEO_DEAGLE],GEO_DEAGLE,0.1f,50,15,15,1.0f,camera.position,Vector3(-2.5f,5.5f,-2.5f));
		camera.cameraRotate.x = 0.f;
		camera.cameraRotate.y = 0.f;
		InitVariables();
	}
}

void Assignment1::Update(double dt)
{
	UpdateDebug(dt);

	rotateAngle = camera.cameraRotate.y;

	UpdateGun(dt);
	UpdateFire(dt);
	UpdateBullet(dt);
	UpdateAmmoBox(dt);
	UpdateEnemy(dt);
	UpdateRestart(dt);
	//camera.Update(dt);

	fps = (float)(1.f / dt);
}

static const float SKYBOXSIZE = 1000.f;
void Assignment1::RenderText(Mesh* mesh, std::string text, Color color)
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
void Assignment1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void Assignment1::RenderMesh(Mesh *mesh, bool enableLight)
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
void Assignment1::RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeY,float sizeX, float x, float y,bool rotate)
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
	if(rotate)
	{
		modelStack.Rotate(rotateAngle,0,0,1);
	}

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
void Assignment1::RenderEnemyOnMap2D(Mesh *mesh, bool enableLight, float sizeY,float sizeX, float x, float y,bool rotate)
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
	if(rotate)
	{
		modelStack.Rotate(-rotateAngle,0,0,1);
	}
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

void Assignment1::RenderAmmoBox(void)
{
	for(unsigned i = 0; i < AmmoBoxes.size(); ++i)
	{
		if(AmmoBoxes[i]->getRender() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(AmmoBoxes[i]->getPos().x,AmmoBoxes[i]->getPos().y,AmmoBoxes[i]->getPos().z);
			modelStack.Scale(AmmoBoxes[i]->getObjScale().x,AmmoBoxes[i]->getObjScale().y,AmmoBoxes[i]->getObjScale().z);
			RenderMesh(AmmoBoxes[i]->getMesh(), AmmoBoxes[i]->getLight());
			modelStack.PopMatrix();
		}
	}
}
void Assignment1::RenderTrees(void)
{
	//float offsetZ = -1000.f;
	float offsetX = 1300.f;

	for(unsigned i = 0; i < NumTree; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(offsetX, (getHeight(m_heightMap,offsetX / 4000.f,TreeoffsetZ[i]/4000.f) * 350.f) + 8.f, TreeoffsetZ[i]);
		modelStack.Scale(10.f,10.f,10.f);
		RenderMesh(meshList[GEO_TREE], bLightEnabled);
		modelStack.PopMatrix();
		offsetX -= 100.f;
	}

	offsetX = 1300.f;

	for(unsigned i = 0; i < NumTree; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(offsetX,(getHeight(m_heightMap,offsetX / 4000.f,(TreeoffsetZ[i] - 100.f)/4000.f) * 350.f) + 8.f, TreeoffsetZ[i] - 100.f);
		modelStack.Scale(10.f,10.f,10.f);
		RenderMesh(meshList[GEO_TREE], bLightEnabled);
		modelStack.PopMatrix();
		offsetX -= 70.f;
	}

	offsetX = 1300.f;

	for(unsigned i = 0; i < NumTree; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(offsetX,(getHeight(m_heightMap,offsetX / 4000.f,(TreeoffsetZ[i] + 100.f)/4000.f)*350.f) + 8.f, TreeoffsetZ[i] + 100.f);
		modelStack.Scale(10.f,10.f,10.f);
		RenderMesh(meshList[GEO_TREE], bLightEnabled);
		modelStack.PopMatrix();
		offsetX -= 75.f;
	}
}
void Assignment1::RenderSkyPlane(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}
void Assignment1::RenderTerrain(void)
{
	modelStack.PushMatrix();
	modelStack.Scale(4000.0f, 350.f, 4000.0f); // values varies.  
	RenderMesh(meshList[GEO_TERRAIN], bLightEnabled);  
	modelStack.PopMatrix();
}
void Assignment1::RenderGun(void)
{
	if(player.getWeapon()->getIndex() == GEO_DEAGLE)
	{
		modelStack.PushMatrix();
		modelStack.Rotate(180,0,0,1);	//Rotation at the start
		modelStack.Translate(-camera.position.x,-camera.position.y,camera.position.z);
		modelStack.Rotate(-camera.cameraRotate.y,0,1,0);
		modelStack.Rotate(camera.cameraRotate.x,1,0,0);
		modelStack.Translate(player.getWeapon()->getGunOffset().x,player.getWeapon()->getGunOffset().y,player.getWeapon()->getGunOffset().z);	//Offset from camera pos
		RenderMesh(player.getWeapon()->getMesh(), false);
		modelStack.PopMatrix();
	}
	else if(player.getWeapon()->getIndex() == GEO_M4A4)
	{
		modelStack.PushMatrix();
		modelStack.Rotate(180,0,1,0);	//Rotation at the start
		modelStack.Translate(-camera.position.x,camera.position.y,-camera.position.z);
		modelStack.Rotate(camera.cameraRotate.y,0,1,0);
		modelStack.Rotate(camera.cameraRotate.x,1,0,0);
		modelStack.Translate(player.getWeapon()->getGunOffset().x,player.getWeapon()->getGunOffset().y,player.getWeapon()->getGunOffset().z);	//Offset from camera pos
		RenderMesh(player.getWeapon()->getMesh(), false);
		modelStack.PopMatrix();
	}
}
void Assignment1::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	//RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}
void Assignment1::RenderHUD(void)
{ 
	RenderMeshIn2D(meshList[GEO_CROSSHAIR],false);
	//Health
	RenderMeshIn2D(meshList[GEO_HEALTHHUD],false,1.5f,1.5f,-67.f,-53.f,false);
	RenderMeshIn2D(meshList[GEO_HEALTHBAR],false,player.getHitpoints() * 0.025f,0.3f,-79.0f,-57.5f,false);

	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << player.getHitpoints();
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 0, 0), 3, 8, 3.7f);

	//Score
	std::ostringstream ss6;
	ss6.precision(5);
	ss6 << "Score: " << score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 0, 1), 3, 30, 2.5f);

	//Gun Info
	std::ostringstream ss4;
	ss4.precision(5);
	ss4 << player.getWeapon()->getCurrentAmmo();
	RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(1, 1, 0), 4, 72, 5);
	std::ostringstream ss5;
	ss5.precision(5);
	ss5 << "\\" << player.getWeapon()->getAmmoCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(1, 1, 0), 4, 75, 5);
	//===================================================//
	//Debug print
	//===================================================//
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 59);
	
	/*std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "x:" << camera.position.x << ",y:" << camera.position.y << "z:" << camera.position.z;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 2, 10);*/
}
void Assignment1::RenderBullet(void)
{
	if(BulletContainer.size() > 0)
	{
		for(unsigned i = 0;i < BulletContainer.size(); ++i)
		{
			if(BulletContainer[i].GetStatus() == true)
			{
				modelStack.PushMatrix();
				modelStack.Translate(BulletContainer[i].getPos().x,BulletContainer[i].getPos().y,BulletContainer[i].getPos().z);
				RenderMesh(BulletContainer[i].getMesh(), false);
				modelStack.PopMatrix();
			}
		}
	}
}
void Assignment1::RenderFloor(void)
{
	//Floor
	modelStack.PushMatrix();
	modelStack.Translate(0,-40,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Scale(SKYBOXSIZE,SKYBOXSIZE,SKYBOXSIZE);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();
}
void Assignment1::RenderLight(void)
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();
}
void Assignment1::RenderDebug(void)
{
	//RenderMesh(meshList[GEO_AXES], false);
}
void Assignment1::RenderMinimap(void)
{
	for(unsigned i = 0;i < enemyContainer.size(); ++i)
	{
		if(enemyContainer[i]->getRender() == true && (enemyContainer[i]->getPos() - camera.position).Length() < 1800)
		{
			RenderEnemyOnMap2D(meshList[GEO_MAP_ENEMY],false,1.5f,1.5f,((enemyContainer[i]->getPos().x - camera.position.x) / 200.f),((enemyContainer[i]->getPos().z - camera.position.z) / 145.f),true);
		}
	}
	RenderMeshIn2D(m_cMinimap->GetAvatar(), false, 20.0f, 10.0f,69.f,49.f);
	//RenderMeshIn2D(m_cMinimap->GetBorder(), false, 20.0f, 20.0f,69.f,49.f,true);
	RenderMeshIn2D(m_cMinimap->GetBackground(), false, 20.0f, 20.0f,69.f,49.f,true);
}
void Assignment1::RenderStaticObjects(void)
{
	for(unsigned i = 0;i < staticObjects.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(staticObjects[i]->getPos().x,staticObjects[i]->getPos().y,staticObjects[i]->getPos().z);
		modelStack.Scale(staticObjects[i]->getObjScale().x,staticObjects[i]->getObjScale().y,staticObjects[i]->getObjScale().z);
		RenderMesh(staticObjects[i]->getMesh(), staticObjects[i]->getLight());
		modelStack.PopMatrix();
	}
}
void Assignment1::RenderEnemy(void)
{
	for(unsigned i = 0;i < enemyContainer.size(); ++i)
	{
		if(enemyContainer[i]->getRender() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(enemyContainer[i]->getPos().x,enemyContainer[i]->getPos().y,enemyContainer[i]->getPos().z);
			modelStack.Rotate(enemyContainer[i]->getDerivedAngle() + 80.f,0,1,0);
			modelStack.Scale(enemyContainer[i]->getObjScale().x,enemyContainer[i]->getObjScale().y,enemyContainer[i]->getObjScale().z);
			RenderMesh(enemyContainer[i]->getMesh(), enemyContainer[i]->getLight());
			modelStack.PopMatrix();
		}
	}
}
// Toggle HUD mode
void Assignment1::SetHUD(const bool m_bHUDmode)
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
void Assignment1::ClearBuffer(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void Assignment1::EnableCamera(void)
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

void Assignment1::Render()
{
	ClearBuffer();
	EnableCamera();

	RenderLight();
	RenderDebug();

	//Rendering world
	//RenderSkybox();
	RenderSkyPlane();
	RenderTerrain();
	RenderFloor();
	RenderTrees();
	RenderEnemy();
	RenderAmmoBox();
	RenderStaticObjects();

	RenderBullet();

	RenderGun();
	//RenderHUD();

	SetHUD(true);
	RenderHUD();
	RenderMinimap();
	SetHUD(false);
}

void Assignment1::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	while(staticObjects.size() > 0)
	{
		C3DObject *object = staticObjects.back();
		delete object;
		staticObjects.pop_back();
	}
	while(AmmoBoxes.size() > 0)
	{
		C3DObject *object = AmmoBoxes.back();
		delete object;
		AmmoBoxes.pop_back();
	}
	while(enemyContainer.size() > 0)
	{
		CEnemy *enemy = enemyContainer.back();
		delete enemy;
		enemyContainer.pop_back();
	}
	if(m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
	if(engine)
	{
		delete engine;
		engine = NULL;
	}
	if(Sound)
	{
		delete Sound;
		Sound = NULL;
	}
	if(camera.engine)
	{
		delete camera.engine;
		camera.engine = NULL;
	}
	if(camera.Sound)
	{
		delete camera.Sound;
		camera.Sound = NULL;
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
