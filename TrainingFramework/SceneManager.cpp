#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "define.h"
#include <iostream>



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

	printf("Init state: %d\n", Camera::GetInstance()->i_state);
	//Box2D
	b2Vec2 gravity = b2Vec2(0.0, 0.0);
	m_world = new b2World(gravity);
	

	//resource
	ResourceManager::GetInstance()->Init();
	FILE* f_SM;
	f_SM = fopen(m_fileSM, "r+");
	if (f_SM != NULL) {
		this->ReadFile(f_SM);
	}
	else std::cout << "Khong tim thay file Scene Manager! \n";

	// 
	for (int i = 0; i < (int) m_ListTerrain.size(); i++) {
		m_ListTerrain[i]->Init();
	}
	//
	m_MainCharacter->Init();
	
	//
	for (int i = 0; i < (int) m_listEnemy.size(); i++) {
		m_listEnemy[i]->Init();
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
		Object* obj;
		int ID, model, Textures, cubeTextures, texture, shader;
		char type[128];
		Vector3 Position, Rotation, Scale;

		fscanf(f_SM, "ID %d\n", &ID);
		std::cout << "ID: " << ID << std::endl;
		fscanf(f_SM, "TYPE %s\n", type);
		Model* pModel;
		if (strcmp(type, "MAIN") == 0) {
			float x, y, w, h, tw, th, speed; char aniFile[128];
			fscanf(f_SM, "COORD %f %f %f %f %f %f\n", &x, &y, &w, &h, &tw, &th);
			pModel = new Model();
			pModel->InitSprite(x, y, w, h, tw, th);
			int num_anim;
			fscanf(f_SM, "ANIMATIONS %d\n", &num_anim);
			if (num_anim > 0) {
				for (int i = 0; i < num_anim; i++) {
					fscanf(f_SM, "animation %s speed %f\n", aniFile, &speed);
					Animation* anim = new Animation(aniFile);
					anim->setAnimationSpeed(speed);
					pModel->addAnimation(anim);
				}
			}
			obj = new MainCharacter(ID);
		}
		else if (strcmp(type, "NPC") == 0) {
			float x, y, w, h, tw, th, speed; char aniFile[128];
			fscanf(f_SM, "COORD %f %f %f %f %f %f\n", &x, &y, &w, &h, &tw, &th);
			pModel = new Model();
			pModel->InitSprite(x, y, w, h, tw, th);
			int num_anim;
			fscanf(f_SM, "ANIMATIONS %d\n", &num_anim);
			if (num_anim > 0) {
				for (int i = 0; i < num_anim; i++) {
					fscanf(f_SM, "animation %s speed %f\n", aniFile, &speed);
					Animation* anim = new Animation(aniFile);
					anim->setAnimationSpeed(speed);
					pModel->addAnimation(anim);
				}
			}
			obj = new NPC(ID);
		}
		else if (strcmp(type, "BULLET") == 0) {
			// do something
		}
		else if (strcmp(type, "GROUND") == 0) {
			// do something
		}
		else {
			fscanf(f_SM, "MODEL %d\n", &model);
			std::cout << "Models: " << model << std::endl;
			pModel = ResourceManager::GetInstance()->GetModelAtID(model);
			pModel->b_IsAnimation = false;
			obj = new Object(ID);
		}
		fscanf(f_SM, "SHADER %d\n", &shader);
		obj->setModel(pModel);
		obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));

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
		obj->InitWVP();
		AddTerrain(obj);
	}
	
	fclose(f_SM);
}

void SceneManager::Update(float deltaTime) {
	if (m_world != NULL) {
		
	}
	Camera::GetInstance()->Update(deltaTime);

	m_MainCharacter->Update(deltaTime);

	for (int i = 0; i < (int) m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->Update(deltaTime);
	}

	for (int i = 0; i < (int) m_listEnemy.size(); i++) {
		m_listEnemy[i]->Update(deltaTime);
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
	for (int i = 0; i < (int) m_ListTerrain.size(); i++) {
		m_ListTerrain[i]->Draw();
	}

	m_MainCharacter->Draw();

	for (int i = 0; i < (int) m_listEnemy.size(); i++) {
		m_listEnemy[i]->Draw();
	}

	for (int i = 0; i < (int) m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->Draw();
	}
}

void SceneManager::AddTerrain(Object * obj) {
	m_ListTerrain.push_back(obj);
}

void SceneManager::AddGun(Bullet* gun) {
	m_ListGun.push_back(gun);
}

void SceneManager::AddBullet(Bullet* bullet) {
	m_listBulletInWorld.push_back(bullet);
}

void SceneManager::RemoveBullet(int index) {
	m_listBulletInWorld[index]->CleanUp();
	m_listBulletInWorld.erase(m_listBulletInWorld.begin() + index);
}

void SceneManager::AddEnemy(Enemy* enemy) {
	m_listEnemy.push_back(enemy);
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
	for (int i = 0; i < (int)m_ListTerrain.size(); i++) {
		m_ListTerrain[i]->CleanUp();
	}

	m_MainCharacter->CleanUp();

	for (int i = 0; i < (int)m_listEnemy.size(); i++) {
		m_listEnemy[i]->CleanUp();
	}

	for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->CleanUp();
	}
}

void SceneManager::Shoot(bool isFront = true) {
	int v = isFront ? 1 : -1;
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	Bullet* bullet = new Bullet(m_ListGun[0]->GetID());
	bullet->Init(0, bullet->GetAttackDame(), bullet->GetAttackSpeed(), bullet->GetSpeedOfBullet().x, bullet->GetSpeedOfBullet().y, bullet->GetMaxOfLength());
	Vector3 posBullet = Vector3(posMainCharacter.x + v * (m_MainCharacter->GetBox().x + m_ListGun[0]->GetBox().x / 2), posMainCharacter.y, 0);
	
	// thieu phan doc textrue cua anh
	
	bullet->SetPosition(posBullet);
	bullet->SetRotation(m_ListGun[0]->GetRotation());
	bullet->SetScale(m_ListGun[0]->GetScale());

	bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
	m_listBulletInWorld.push_back(bullet);
}

void SceneManager::ChangeGun(bool isEmptyBullet = true) {
	Bullet *bullet = m_ListGun[0];
	if (isEmptyBullet) {
		m_ListGun.erase(m_ListGun.begin());
		m_ListGun.push_back(bullet);
	}
	else {
		m_ListGun[0] = m_ListGun[1];
		m_ListGun[1] = bullet;
	}
}

void SceneManager::SetStateHellGun(Bullet* hellBullet, float enemyWidth, bool isFront = true) {
	int v = isFront ? 1 : -1;
	for (int i = 0; i < 3; i++) {
		b2Vec2 posHellBullet = hellBullet->getBody()->GetPosition();
		Bullet* bullet = new Bullet(hellBullet->GetID());
		bullet->Init(0, hellBullet->GetAttackDame(), hellBullet->GetAttackSpeed(), hellBullet->GetSpeedOfBullet().x, hellBullet->GetSpeedOfBullet().x / 2 * (i - 2), hellBullet->GetMaxOfLength());
		Vector3 posBullet = Vector3(posHellBullet.x + v * (hellBullet->GetBox().x + enemyWidth), posHellBullet.y, 0);

		// thieu phan doc textrue cua anh

		bullet->SetPosition(posBullet);
		bullet->SetRotation(m_ListGun[0]->GetRotation());
		bullet->SetScale(m_ListGun[0]->GetScale());

		bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
		m_listBulletInWorld.push_back(bullet);
	}
}