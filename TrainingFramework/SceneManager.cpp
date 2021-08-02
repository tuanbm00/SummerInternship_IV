#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "defines.h"
#include <Collision/Shapes/b2CircleShape.h>
#include <Dynamics/b2World.h>
#include <iostream>
#include <Box2D/Dynamics/b2Body.h>


SceneManager::SceneManager(char* fileSM)
{
	m_fileSM = fileSM;
}


SceneManager::~SceneManager()
{
}

void SceneManager::SetFileManager(char* fileSM) {
	m_fileSM = fileSM;
}

void SceneManager::Init() {
	m_Object.clear();

	//Box2D
	b2Vec2 gravity = b2Vec2(0.0, 8.0);
	//physicWorld = new b2World(gravity);
	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	ResourceManager::GetInstance()->Init();
	FILE* f_SM;
	f_SM = fopen(m_fileSM, "r+");
	if (f_SM != NULL) {
		this->ReadFile(f_SM);
	}
	else std::cout << "Khong tim thay file Scene Manager! \n";

	for (int i = 0; i < m_Object.size(); i++) {
		m_Object[i]->Init();
	}
}

void SceneManager::ReadFile(FILE* f_SM)
{
	//Cameras
	int numOfCameras;
	fscanf(f_SM, "#Cameras: %d\n", &numOfCameras);
	for (register int i = 0; i < numOfCameras; i++) {
		int ID;
		Vector3 Position, Target, Up;
		float Fovy, Near, Far, Move_Speed, Rotate_Speed;
		fscanf(f_SM, "ID %d\n", &ID);
		fscanf(f_SM, "POSITION %f %f %f\n", &Position.x, &Position.y, &Position.z);
		fscanf(f_SM, "TARGET %f %f %f\n", &Target.x, &Target.y, &Target.z);
		fscanf(f_SM, "UP %f %f %f\n", &Up.x, &Up.y, &Up.z);
		fscanf(f_SM, "FOVY %f\n", &Fovy);
		fscanf(f_SM, "NEAR %f\n", &Near);
		fscanf(f_SM, "FAR %f\n", &Far);
		fscanf(f_SM, "MOVE_SPEED %f\n", &Move_Speed);
		fscanf(f_SM, "ROTATE_SPEED %f\n", &Rotate_Speed);
			
		Camera::GetInstance()->Init(Fovy, Near, Far, Move_Speed, Rotate_Speed);
		Camera::GetInstance()->SetPosition(Position);
		Camera::GetInstance()->SetTarget(Target);
		Camera::GetInstance()->SetUp(Up);
	}

	//Lights
	int numOfLights;
	fscanf(f_SM, "#Lights: %d\n", &numOfLights);
	for (register int i = 0; i < numOfLights; i++) {
		int ID;
		char Type[50];
		Vector3 Dir, Color;
		fscanf(f_SM, "ID %d\n", &ID);
		fscanf(f_SM, "TYPE %s\n", &Type);
		fscanf(f_SM, "POS_DIR %f %f %f\n", &Dir.x, &Dir.y, &Dir.z);
		fscanf(f_SM, "COLOR %f %f %f\n", &Color.x, &Color.y, &Color.z);
	}

	//Object
	int numOfObjects;
	fscanf(f_SM, "#Objects: %d\n", &numOfObjects);
	for (register int i = 0; i < numOfObjects; i++) {
		int ID, model, Textures, cubeTextures, texture, shader;
		Vector3 Position, Rotation, Scale;

		fscanf(f_SM, "ID %d\n", &ID);
		std::cout << "ID: " << ID << std::endl;
		fscanf(f_SM, "MODEL %d\n", &model);
		std::cout << "Models: " << model << std::endl;
		fscanf(f_SM, "SHADER %d\n", &shader);
		std::shared_ptr<Object> obj = std::make_shared<Object>(ID, (ResourceManager::GetInstance()->GetShaderAtID(shader)), ResourceManager::GetInstance()->GetModelAtID(model));

		fscanf(f_SM, "TEXTURES %d\n", &Textures);
		for (register int j = 0; j < Textures; j++) {
			fscanf(f_SM, "TEXTURE %d\n", &texture);
			//Add Texture here
			std::cout << "Texture: " << texture << std::endl;
			obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
		}
		fscanf(f_SM, "CUBETEXTURES %d\n", &cubeTextures);
		for (register int j = 0; j < cubeTextures; j++) {
			fscanf(f_SM, "TEXTURE %d\n", &texture);
			//Add Cube Texture here
			std::cout << "CUBE Texture: " << texture << std::endl;
			obj->SetCubeTexture(ResourceManager::GetInstance()->GetCubeTextureAtID(texture));
		}

		fscanf(f_SM, "POSITION %f %f %f\n", &Position.x, &Position.y, &Position.z);
		fscanf(f_SM, "ROTATION %f %f %f\n", &Rotation.x, &Rotation.y, &Rotation.z);
		fscanf(f_SM, "SCALE %f %f %f\n", &Scale.x, &Scale.y, &Scale.z);
		
		obj->SetPosition(Position);
		obj->SetScale(Scale);
		obj->SetRotation(Rotation);
		AddObject(obj);

	}
	
	fclose(f_SM);
}

void SceneManager::Update(float deltaTime) {
	
	Camera::GetInstance()->Update(deltaTime);
	for (int i = 0; i < m_Object.size(); i++) {
		m_Object[i]->Update(deltaTime);
	}

	//physicWorld->Step(deltaTime, velocityIterations, positionIterations);

	static float timeStep = 0.8f;
	timeStep += deltaTime;

	if (m_bIsFighting && timeStep >= 0.8f) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/laser.mp3", false);
		printf("Fighting to Target at: %f - %f!\n", m_oTarget.x, m_oTarget.y);
		timeStep -= 0.8f;
	}

	if (timeStep >= 0.8f) {
		timeStep = 0.8f;
	}
	//this->CheckOnCollision();
}

void SceneManager::Draw() {
	for (int i = 0; i < m_Object.size(); i++) {
		m_Object[i]->Draw();
	}
}

void SceneManager::AddObject(std::shared_ptr<Object> obj) {
	m_Object.push_back(obj);
}

void SceneManager::Key(unsigned char key, bool bIsPressed) {
	Camera::GetInstance()->Key(key, bIsPressed);
}

void SceneManager::OnMouseButtonDown(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
		printf("Left Mouse Button Down! %d - %d!\n", X, Y);
		SetIsFighting(true);
		m_oTarget = Vector2(X, Y);
	}
	break;
	case RMB:
	{
		printf("Right Mouse Button Down! %d - %d!\n", X, Y);
	}
	break;
	case 2:
	{
		printf("Left Mouse Button Click! %d - %d!\n", X, Y);
	}
	break;
	case 3:
	{
		printf("Right Mouse Button Click! %d - %d!\n", X, Y);
	}
	break;
	}
}

void SceneManager::OnMouseButtonUp(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
		SetIsFighting(false);
		m_oTarget = Vector2(X, Y);
		printf("Left Mouse Button Up! %d - %d!\n", X, Y);
	}
	break;
	case RMB:
	{
		printf("Right Mouse Button Up! %d - %d!\n", X, Y);
	}
	break;
	}
}

void SceneManager::OnMouseButtonMove(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
		printf("Left Mouse Moving: %d - %d!\n", X, Y);
		m_oTarget = Vector2(X, Y);
	}
	break;
	case RMB:
	{
		printf("Right Mouse Moving: %d - %d!\n", X, Y);
	}
	break;
	}
}

void SceneManager::SetIsFighting(bool IsFighting) {
	m_bIsFighting = IsFighting;
}

void SceneManager::CleanUp() {
	Camera::GetInstance()->CleanUp();
	for (int i = 0; i < m_Object.size(); i++) {
		m_Object[i]->CleanUp();
	}
}