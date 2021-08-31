#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "define.h"
#include <iostream>
#include "Globals.h"
#include "GameplayUI.h"
#include "Healthy.h"
#include <random>
#include <time.h>


SceneManager::SceneManager(int currentLevel)
{
	filterMain.categoryBits = CATEGORY_PLAYER;
	filterMain.maskBits = MASK_PLAYER;
	filterMain.groupIndex = -1;
	filterRoll.categoryBits = CATEGORY_PLAYER;
	filterRoll.maskBits = MASK_PLAYER;
	filterRoll.groupIndex = -2;
	filterBoomerang1.categoryBits = CATEGORY_BULLET_PLAYER;
	filterBoomerang1.maskBits = MASK_BULLET_PLAYER;
	filterBoomerang1.groupIndex = -1;
	filterBoomerang2.categoryBits = CATEGORY_BULLET_PLAYER;
	filterBoomerang2.maskBits = MASK_BULLET_PLAYER;
	filterBoomerang2.groupIndex = -2;

	m_currentLevel = currentLevel;
}


SceneManager::~SceneManager()
{
}

void SceneManager::SetFileManager(char* fileSM, char* fileMAP) {
	m_fileSM = fileSM;
	m_fileMAP = fileMAP;
	m_direction = 1;
	m_Horizontal = 0.0f;
	m_Vertical = 0.0f;
	m_shoot = 0.0f;
	m_time = 50.0f;
	m_timeChangeGun = 50.0f;
	m_timeHurt = 0.5f;
	keyPressed = 0;
	m_time_roll = 0;
	is_roll = false;
	roll_step = 0;
}

void SceneManager::ChangeToResultScreen(bool bIsVictory)
{
	GameStateMachine::GetInstance()->PushState(StateTypes::GS_RESULT, bIsVictory, m_currentLevel);
}

void SceneManager::Init() {
	srand(time(NULL));

	timeCount = 0.0f;
	numJump = 0;
	jumpstep = 0;
	is_in_ground = false;
	
	//Box2D
	b2Vec2 gravity = b2Vec2(0.0, 0.0);
	m_world = new b2World(gravity);


	//resource
	m_boss = NULL;
	m_IsBossAppear = false;
	m_IsTowerDefend = false;
	FILE* f_SM;
	fopen_s(&f_SM, m_fileSM, "r+");
	if (f_SM != NULL) {
		this->ReadFile(f_SM);
	}
	FILE* f_MAP;
	fopen_s(&f_MAP, m_fileMAP, "r+");
	if (f_MAP != NULL) {
		this->ReadMap(f_MAP);
	}
	//
	mainIcon = new Object(101);
	Model * ppmodel = new Model();
	ppmodel->b_IsAnimation = true;
	ppmodel->InitSprite(0, 0, 50, 50, 200, 100);
	ppmodel->addAnimation(ResourceManager::GetInstance()->GetAnimationAtID(28));
	mainIcon->setModel(ppmodel);
	mainIcon->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(31));
	mainIcon->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	mainIcon->SetPosition(0, 0, 0);
	mainIcon->SetScale(3, 3, 1);
	mainIcon->m_current_anim = 1;
	mainIcon->InitWVP();
	Singleton<GameplayUI>::GetInstance()->SetCurrentLevel(m_currentLevel);
	Singleton<GameplayUI>::GetInstance()->Init(); //Init GameplayUI
	Singleton<GameplayUI>::GetInstance()->SetMainCharacter(m_MainCharacter); //Set MainCharacter to show information's MC
	LoadDecor();
}

void SceneManager::ReadFile(FILE* f_SM)
{
	//Cameras
	int numOfCameras;
	fscanf_s(f_SM, "#Cameras: %d\n", &numOfCameras);
	for (register int i = 0; i < numOfCameras; ++i) {
		int ID;
		Vector3 Position, Target, Up;
		float Fovy, Near, Far, Move_Speed, Rotate_Speed;
		fscanf_s(f_SM, "ID %d\n", &ID);
		fscanf_s(f_SM, "POSITION %f %f %f\n", &Position.x, &Position.y, &Position.z);
		fscanf_s(f_SM, "TARGET %f %f %f\n", &Target.x, &Target.y, &Target.z);
		fscanf_s(f_SM, "UP %f %f %f\n", &Up.x, &Up.y, &Up.z);
		fscanf_s(f_SM, "FOVY %f\n", &Fovy);
		fscanf_s(f_SM, "NEAR %f\n", &Near);
		fscanf_s(f_SM, "FAR %f\n", &Far);
		fscanf_s(f_SM, "MOVE_SPEED %f\n", &Move_Speed);
		fscanf_s(f_SM, "ROTATE_SPEED %f\n\n", &Rotate_Speed);

		Camera::GetInstance()->Init(Fovy, Near, Far, Move_Speed, Rotate_Speed);
		Camera::GetInstance()->SetPosition(Position);
		Camera::GetInstance()->SetTarget(Target);
		Camera::GetInstance()->SetUp(Up);
	}

	//Object
	int numOfObjects;
	fscanf_s(f_SM, "#Objects: %d\n", &numOfObjects);
	for (register int i = 0; i < numOfObjects; ++i) {
		int ID, texture, shader, anim, bulletID;
		int bullet1, bullet2, bullet3, bullet4;
		float b1, b2;
		char type[128];
		Vector3 Position, Rotation, Scale;

		fscanf_s(f_SM, "ID %d\n", &ID);
		fscanf_s(f_SM, "TYPE %s\n", type, _countof(type));
		Model* pModel;
		float x, y, w, h, tw, th;
		float dame, attack, speedx, speedy, dis, hp;
		fscanf_s(f_SM, "COORD %f %f %f %f %f %f\n", &x, &y, &w, &h, &tw, &th);
		pModel = new Model();
		int num_anim;
		fscanf_s(f_SM, "ANIMATIONS %d\n", &num_anim);
		if (num_anim > 0) {
			pModel->b_IsAnimation = true;
			for (int i = 0; i < num_anim; ++i) {
				fscanf_s(f_SM, "ANIMATION %d\n", &anim);
				pModel->addAnimation(ResourceManager::GetInstance()->GetAnimationAtID(anim));
			}
		}
		pModel->InitSprite(x, y, w, h, tw, th);
		fscanf_s(f_SM, "SHADER %d\n", &shader);
		fscanf_s(f_SM, "TEXTURE %d\n", &texture);

		if (strcmp(type, "GUN_PLAYER") == 0 || strcmp(type, "GUN_ENEMY") == 0 || strcmp(type, "GUN_BOSS") == 0) {
			fscanf_s(f_SM, "BULLET %f %f %f %f %f\n", &dame, &attack, &speedx, &speedy, &dis);
		}
		else if (strcmp(type, "ENEMY") == 0) {
			fscanf_s(f_SM, "CHARACTER %f %f %f\n", &hp, &speedx, &speedy);
			fscanf_s(f_SM, "BULLET_ID %d\n", &bulletID);
			fscanf_s(f_SM, "BOX %f %f\n", &b1, &b2);
		}
		else if (strcmp(type, "BOSS") == 0) {
			fscanf_s(f_SM, "CHARACTER %f %f %f\n", &hp, &speedx, &speedy);
			fscanf_s(f_SM, "BULLET_ID %d %d %d %d\n", &bullet1, &bullet2, &bullet3, &bullet4);
		}
		else {
			fscanf_s(f_SM, "CHARACTER %f %f %f\n", &hp, &speedx, &speedy);
		}
			//Add Texture here

		fscanf_s(f_SM, "POSITION %f %f %f\n", &Position.x, &Position.y, &Position.z);
		fscanf_s(f_SM, "ROTATION %f %f %f\n", &Rotation.x, &Rotation.y, &Rotation.z);
		fscanf_s(f_SM, "SCALE %f %f %f\n\n", &Scale.x, &Scale.y, &Scale.z);


		if (strcmp(type, "MAIN") == 0) {
			m_MainCharacter = new MainCharacter(ID);
			m_MainCharacter->setModel(pModel);
			m_MainCharacter->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			m_MainCharacter->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			m_MainCharacter->SetPosition(Position);
			m_MainCharacter->SetScale(Scale);
			m_MainCharacter->SetRotation(Rotation);
			m_MainCharacter->SetMaxHP(hp);
			m_MainCharacter->SetBodyObject(m_MainCharacter->GetPosition().x, m_MainCharacter->GetPosition().y, m_world);
			m_MainCharacter->InitWVP();

			// set HP texture
			Model* modelHP = new Model();
			modelHP->InitSprite(0, 0, 300, 30, 300, 30, false);
			m_ModelDump.push_back(modelHP);
			Healthy* whiteHp = new Healthy(0);
			whiteHp->setModel(modelHP);
			whiteHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
			whiteHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(WHITE));
			whiteHp->SetPosition(m_MainCharacter->GetPosition());
			whiteHp->SetOriginScale(1, 1, 1);
			whiteHp->SetRotation(m_MainCharacter->GetRotation());
			whiteHp->InitWVP();
			Healthy* redHp = new Healthy(1);
			redHp->setModel(modelHP);
			redHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
			redHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(RED));
			redHp->SetPosition(m_MainCharacter->GetPosition());
			redHp->SetOriginScale(1, 1, 1);
			redHp->SetRotation(m_MainCharacter->GetRotation());
			redHp->InitWVP();

			m_ObjectDump.push_back(redHp);
			m_ObjectDump.push_back(whiteHp);
			m_MainCharacter->SetHPTexture(whiteHp);
			m_MainCharacter->SetHPTexture(redHp, false);
			m_MainCharacter->resetAnimation(dead);
		}
		else if (strcmp(type, "ENEMY") == 0) {
			Enemy* enemy = new Enemy(ID);
			enemy->setModel(pModel);
			enemy->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			enemy->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			enemy->SetBulletID(bulletID);
			enemy->SetPosition(Position);
			enemy->SetScale(Scale);
			enemy->SetRotation(Rotation);
			enemy->SetMaxHP(hp);
			enemy->SetSpeed(speedx, speedy);
			enemy->setTransBox(b1, b2);
			enemy->InitWVP();

			m_listEnemy.push_back(enemy);
		}
		else if (strcmp(type, "BOSS") == 0) {
			m_boss = new Boss(ID);
			m_boss->setModel(pModel);
			m_boss->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			m_boss->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			m_boss->SetPosition(Position);
			m_boss->SetScale(Scale);
			m_boss->SetRotation(Rotation);
			m_boss->SetMaxHP(hp);
			m_boss->SetSpeed(speedx, speedy);
			m_boss->InitWVP();
			m_boss->AddBulletID(bullet1);
			m_boss->AddBulletID(bullet2);
			m_boss->AddBulletID(bullet3);
			m_boss->AddBulletID(bullet4);
			m_IsBossAppear = false;
		}
		else if (strcmp(type, "GUN_PLAYER") == 0) {
			Bullet* bullet = new Bullet(ID);
			bullet->setModel(pModel);
			bullet->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			bullet->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			bullet->SetPosition(Position);
			bullet->SetScale(Scale);
			bullet->SetRotation(Rotation);
			bullet->InitA(dame, attack, speedx, speedy, dis);
			bullet->InitWVP();
			AddGun(bullet);
		}
		else if (strcmp(type, "GUN_ENEMY") == 0) {
			Bullet* bullet = new Bullet(ID);
			bullet->setModel(pModel);
			bullet->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			bullet->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			bullet->SetPosition(Position);
			bullet->SetScale(Scale);
			bullet->SetRotation(Rotation);
			bullet->InitA(dame, attack, speedx, speedy, dis);
			bullet->InitWVP();
			m_ListGunOfEnemy.push_back(bullet);
		}
		else {
			// do something
		}
	}

	fclose(f_SM);

	// test sung
}

void SceneManager::ReadMap(FILE *f_MAP) {
	groundTest = new Ground();
	FILE * fp;
	fopen_s(&fp, "../Resources/Map/map.txt", "r+");
	int ni; fscanf_s(fp, "%d\n", &ni);
	Vector4 * Omap = new Vector4[ni];
	for (int i = 0; i < ni; ++i) {
		float x, y, w, h;
		fscanf_s(fp, "%f %f %f %f\n", &x, &y, &w, &h);
		Omap[i] = Vector4(x, y, w, h);
	}
	fclose(fp);
	char filetex[128];
	float tw = 0, th = 0;
	if (m_currentLevel == 1) {
		strcpy_s(filetex, "../Resources/Map/mapT1.tga");
		tw = 3200.0f; th = 400.0f;
	}
	if (m_currentLevel == 2) {
		strcpy_s(filetex, "../Resources/Map/mapT2.tga");
		tw = 3200.0f;
		th = 400.0f;
	}
	if (m_currentLevel == 3) {
		strcpy_s(filetex, "../Resources/Map/mapT3.tga");
		tw = 3200.0f;
		th = 400.0f;
	}
	if (m_currentLevel == 4) {
		strcpy_s(filetex, "../Resources/Map/mapT4.tga");
		tw = 3200.0f;
		th = 400.0f;
	}
	Texture * texx = new Texture(99, filetex);
	texx->InitA();
	groundTest->setTexture(texx);
	groundTest->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	//
	int width, height, row, col, xi, num, index;
	float len;

	fscanf_s(f_MAP, "%d %d\n", &width, &height);
	fscanf_s(f_MAP, "%d %f\n", &index, &len);
	// background
	float para = 0.0f;
	for (int i = 0; i < index; ++i) {
		fscanf(f_MAP, "%d", &xi);
		Object * background = new Object(123);
		Model * backgroundModel = new Model();
		backgroundModel->InitSprite(0, 0, width, height, width, height);
		background->setModel(backgroundModel);
		ResourceManager::GetInstance()->GetBackgroundAtID(xi)->Init();
		background->SetTexture(ResourceManager::GetInstance()->GetBackgroundAtID(xi));
		background->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
		background->SetPosition(Camera::GetInstance()->GetPosition().x + len, Camera::GetInstance()->GetPosition().y - 500, 0);
		background->SetScale(2, 2, 1);
		background->start(para, len);
		background->InitWVP();
		m_ListBackground.push_back(background);
		para += 0.2f;
	}
	//
	fscanf(f_MAP, "\n");
	fscanf_s(f_MAP, "%d %d\n", &row, &col);
	Camera::GetInstance()->setLimitX(-WIDTH*col/2, WIDTH*(col/2-1));
	Camera::GetInstance()->setLimitY(-WIDTH * row / 2, WIDTH*(row / 2 - 1));
	for (int i = 0; i < row; ++i) {
		std::vector<int> line;
		std::vector<int> isLine;
		std::vector<Terrain*> lineMap;
		for (int j = 0; j < col; ++j) {
			fscanf_s(f_MAP, "%d ", &xi);
			line.push_back(xi);
			Terrain* terrain = NULL;
			if (xi >= 0) {
				terrain = new Terrain(xi);
				terrain->SetPosition(WIDTH *(j - col / 2), WIDTH*(i - row / 2));
				if (xi <= 15) {
					terrain->m_bIsTerrain = true;
				}
				else if(xi <= 19){
					terrain->m_bIsTerrain = false;
				}
				Vector2 origin = Vector2(-WIDTH *(j - col / 2), -WIDTH*(i - row / 2));
				groundTest->addVertex(Omap[xi].x, Omap[xi].y, Omap[xi].z, Omap[xi].w, tw, th, origin);
			}
			isLine.push_back(0);
			lineMap.push_back(terrain);
		}
		fscanf_s(f_MAP, "\n");
		isInit.push_back(isLine);
		map.push_back(line);
		m_listTerrain.push_back(lineMap);
	}
	delete[] Omap;
	groundTest->Init();
	

	int numOfEnemy, id, posRow, posCol, left, right;
	fscanf_s(f_MAP, "#Enemy: %d\n", &numOfEnemy);
	for (int i = 0; i < numOfEnemy; ++i) {
		fscanf_s(f_MAP, "%d %d %d %d %d\n", &id, &posRow, &posCol, &left, &right);
		mapEnemy[{posRow, posCol}] = id + 1;
		mapLimit[{posRow, posCol}] = { left, right };
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < (int)m_ListGunOfEnemy.size(); ++j) {
			int id = m_boss->GetBulletID(i);
			if (id == -1) {
				continue;
			}
			if (m_ListGunOfEnemy[j]->GetID() == id) {
				m_boss->AddBullet(m_ListGunOfEnemy[j]);
				break;
			}
		}
	}
	fclose(f_MAP);
}

void SceneManager::LoadDecor()
{
	float imageW, imageH;
	char texFile[128], decorFile[128];
	if (m_currentLevel == 1) {
		strcpy_s(texFile, "../Resources/Map/mapDecor1.tga");
		strcpy_s(decorFile, "../Resources/Map/decor1.txt");
		imageH = 1400.0f; imageW = 3200.0f;
	}
	if (m_currentLevel == 2) {
		strcpy_s(texFile, "../Resources/Map/mapDecor2.tga");
		strcpy_s(decorFile, "../Resources/Map/decor2.txt");
		imageH = 1000.0f; imageW = 3200.0f;
	}
	if (m_currentLevel == 3) {
		strcpy_s(texFile, "../Resources/Map/mapDecor3.tga");
		strcpy_s(decorFile, "../Resources/Map/decor2.txt");
		imageH = 1000.0f; imageW = 3200.0f;
	}
	if (m_currentLevel == 4) {
		strcpy_s(texFile, "../Resources/Map/mapDecor4.tga");
		strcpy_s(decorFile, "../Resources/Map/decor4.txt");
		imageH = 400.0f; imageW = 1000.0f;
	}
	m_Decor = new Ground;
	Texture * tex = new Texture(1, texFile);
	tex->InitA();
	m_Decor->setTexture(tex);
	m_Decor->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	FILE * fp;
	fopen_s(&fp, decorFile, "r");
	int num;
	int row, col, sizex, sizey, x, y, w, h;
	fscanf_s(fp, "%d\n", &num);
	if (num == 0) return;
	fscanf_s(fp, "%d %d\n", &row, &col);
	for (int i = 0; i < num; ++i) {
		fscanf_s(fp, "%d %d %d %d %d %d\n", &sizex, &sizey, &x, &y, &w, &h);
		Vector2 origin = Vector2(-WIDTH * (sizey - col / 2), -WIDTH * (sizex - row / 2));
		m_Decor->addVertex(x, y, w, h, imageW, imageH, origin);
	}
	fclose(fp);
	m_Decor->Init();

}

int cnt = 0;
void SceneManager::Draw() {

	glUseProgram(ResourceManager::GetInstance()->GetShaderAtID(0)->program);

	for (int i = m_ListBackground.size() - 1; i >= 0; --i) m_ListBackground[i]->Draw();
	
	groundTest->Draw();
	m_Decor->Draw();

	Vector3 pos = Camera::GetInstance()->GetPosition();
	m_ListGunOfPlayer[0]->SetPosition(pos.x - 1090, pos.y - 800, 0);
	m_ListGunOfPlayer[1]->SetPosition(pos.x - 1090, pos.y - 700, 0);
	mainIcon->SetPosition(pos.x - 1200, pos.y - 750, 0);
	mainIcon->Draw();
	

	for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
		if (m_listEnemyInWorld[i]->checkDraw()) {
			m_listEnemyInWorld[i]->Draw();
			m_listEnemyInWorld[i]->DrawHP();
		}
	}
	for (int i = 0; i < m_listEnemyDead.size(); ++i) {
		m_listEnemyDead[i]->Draw();
	}
//	m_listEnemy[0]->Draw();
	if (m_IsBossAppear == true && m_boss != NULL) {
		m_boss->Draw();
		m_boss->DrawHP();
	}

	for (int i = 0; i < (int)m_listBulletInWorld.size(); ++i) {
		m_listBulletInWorld[i]->Draw();
	}

	for (int i = 0; i < 2; ++i) {
		m_ListGunOfPlayer[i]->m_current_anim = 0;
		m_ListGunOfPlayer[i]->getModel()->resetTexture();
		m_ListGunOfPlayer[i]->Draw();
	}
	
	m_MainCharacter->Draw();
	m_MainCharacter->DrawHP();

	Singleton<GameplayUI>::GetInstance()->Draw(); //Draw GameplayUI
	if (m_bIsVictory) Singleton<GameplayUI>::GetInstance()->DrawVictory();
	if(m_MainCharacter->getDead()) Singleton<GameplayUI>::GetInstance()->DrawGameOver();
}


void SceneManager::AddGun(Bullet* gun) {
	m_ListGunOfPlayer.push_back(gun);
}

void SceneManager::AddBullet(Bullet* bullet) {
	m_listBulletInWorld.push_back(bullet);
}

void SceneManager::RemoveBullet(int index) {
	m_world->DestroyBody(m_listBulletInWorld[index]->getBody());
	delete m_listBulletInWorld[index];
	m_listBulletInWorld.erase(m_listBulletInWorld.begin() + index);
}

void SceneManager::AddEnemy(Enemy* enemy) {
	m_listEnemyInWorld.push_back(enemy);
}



void SceneManager::OnMouseButtonDown(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
	}
	break;
	case RMB:
	{
	}
	break;
	case 2:
	{
	}
	break;
	case 3:
	{
	}
	break;
	}
}

void SceneManager::OnMouseButtonUp(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{

	}
	break;
	case RMB:
	{

	}
	break;
	}
	Singleton<GameplayUI>::GetInstance()->OnMouseButtonUp(X, Y, Button);
}

void SceneManager::OnMouseButtonMove(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
	}
	break;
	case RMB:
	{
	}
	break;
	}
}


void SceneManager::CleanUp() {
	ResourceManager::GetInstance()->CleanDump();
	Singleton<GameplayUI>::GetInstance()->CleanUp();
	for (int i = 0; i < (int)m_listTerrain.size(); ++i) {
		for (int j = 0; j < (int)m_listTerrain[i].size(); ++j) {
			delete m_listTerrain[i][j];
		}
	}
	for (int i = 0; i < (int)m_ListBackground.size(); ++i) {
		m_ListBackground[i]->deleteTex();
		delete m_ListBackground[i]->getModel();
		delete m_ListBackground[i];
	}
	delete m_MainCharacter->getModel();
	delete m_MainCharacter;

	if (m_boss) {
		delete m_boss->getModel();
		delete m_boss;
	}
	for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
		delete m_listEnemyInWorld[i]->getModel();
		delete m_listEnemyInWorld[i];
	}

	for (int i = 0; i < (int)m_listEnemy.size(); ++i) {
		delete m_listEnemy[i]->getModel();
		delete m_listEnemy[i];
	}

	for (int i = 0; i < (int)m_ListGunOfPlayer.size(); ++i) {
		delete m_ListGunOfPlayer[i]->getModel();
		delete m_ListGunOfPlayer[i];
	}
	for (int i = 0; i < (int)m_ListGunOfEnemy.size(); ++i) {
		delete m_ListGunOfEnemy[i]->getModel();
		delete m_ListGunOfEnemy[i];
	}
	for (int i = 0; i < (int)m_listBulletInWorld.size(); ++i) {
		delete m_listBulletInWorld[i];
	}
	for (int i = 0; i < m_ObjectDump.size(); ++i) delete m_ObjectDump[i];
	for (int i = 0; i < m_ModelDump.size(); ++i) delete m_ModelDump[i];
	delete mainIcon->getModel();
	delete mainIcon;
	delete groundTest;
	delete m_Decor;
	delete m_world;
}

void SceneManager::Shoot() {
	// set bullet
	m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 4);
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	Bullet* bullet = new Bullet(m_ListGunOfPlayer[0]->GetID());
	float dame = m_ListGunOfPlayer[0]->GetAttackDame();
	if (m_ListGunOfPlayer[0]->GetNumberOfBullet() == 1) {
		dame *= 2;
	}
	else {
		if (rand() % 25 == 8) {
			dame *= 1.5;
		}
	}
	bullet->InitA(dame, m_ListGunOfPlayer[0]->GetAttackSpeed(), m_direction*m_ListGunOfPlayer[0]->GetSpeedOfBullet().x, m_ListGunOfPlayer[0]->GetSpeedOfBullet().y, m_ListGunOfPlayer[0]->GetMaxOfLength());
	Vector3 posBullet = Vector3(posMainCharacter.x + m_direction * (m_MainCharacter->GetBox().x + m_ListGunOfPlayer[0]->GetBox().x / 2), posMainCharacter.y, 0);

	if (m_ListGunOfPlayer[0]->GetID() == CATEGORY_QUICK_PISTOL) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/pistol2.wav", false);
	}

	if (m_ListGunOfPlayer[0]->GetID() == CATEGORY_BOOMERANG) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boomerang.wav", false);
	}

	if (m_ListGunOfPlayer[0]->GetID() == CATEGORY_HELL_GUN) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hellgun.wav", false);
	}
	// set follow bazoka
	if (m_ListGunOfPlayer[0]->GetID() == CATEGORY_BAZOKA) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bazooka3.wav", false);
		float minLength = 2000;
		for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
			if (m_direction * m_MainCharacter->GetPosition().x < m_direction * m_listEnemyInWorld[i]->GetPosition().x) {
				if (m_MainCharacter->GetPosition().y + 800 > m_listEnemyInWorld[i]->GetPosition().y && m_MainCharacter->GetPosition().y - 400 < m_listEnemyInWorld[i]->GetPosition().y) {
					float high = abs(m_MainCharacter->GetPosition().y - m_listEnemyInWorld[i]->GetPosition().y);
					float dis = abs(m_MainCharacter->GetPosition().x - m_listEnemyInWorld[i]->GetPosition().x);
					if (minLength > high && dis < 2000.0f) {
						minLength = high;
						bullet->SetTarget(m_listEnemyInWorld[i]->getBody());
					}
				}
			}
		}
	}

	bullet->SetIsChange();
	bullet->setModel(m_ListGunOfPlayer[0]->getModel());
	bullet->setShader(m_ListGunOfPlayer[0]->getShaders());
	bullet->SetTexture(m_ListGunOfPlayer[0]->getTexture());
	bullet->SetPosition(posBullet);
	bullet->SetScale(m_ListGunOfPlayer[0]->GetScale());
	bullet->SetRotation(m_ListGunOfPlayer[0]->GetRotation());
	bullet->InitWVP();
	bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
	bullet->m_current_anim = m_direction;

	AddBullet(bullet);
}

void SceneManager::EnemyAttack(Enemy* enemy) {
	Vector2 box = enemy->GetBox();
	//box.x /= 2; box.y /= 2;
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	b2Vec2 posEnemy = enemy->getBody()->GetPosition();
	float dir = posMainCharacter.x > posEnemy.x ? 1.0f : -1.0f;
	Vector3 posBullet = Vector3(posEnemy.x, posEnemy.y, 0);
	if (enemy->GetBullet()->GetID() == CATEGORY_RADIATE_GUN) {
		for (int i = 0; i < 3; ++i) {
			Bullet* bullet = new Bullet(enemy->GetBullet()->GetID());
			bullet->InitA(enemy->GetBullet()->GetAttackDame(), enemy->GetBullet()->GetAttackSpeed(), dir*enemy->GetBullet()->GetSpeedOfBullet().x, enemy->GetBullet()->GetSpeedOfBullet().x / 2 * (i -1), enemy->GetBullet()->GetMaxOfLength());

			bullet->SetIsChange();
			bullet->setModel(enemy->GetBullet()->getModel());
			bullet->setShader(enemy->GetBullet()->getShaders());
			bullet->SetTexture(enemy->GetBullet()->getTexture());
			bullet->SetPosition(posBullet);
			bullet->SetScale(enemy->GetBullet()->GetScale());
			bullet->SetRotation(enemy->GetBullet()->GetRotation());
			bullet->InitWVP();
			bullet->SetBodyObject(posBullet.x, posBullet.y, m_world, false);
			bullet->m_current_anim = Idle * enemy->m_direction;

			AddBullet(bullet);
		}
	}
	else {
		float scale = (posEnemy.y - posMainCharacter.y) / (posEnemy.x - posMainCharacter.x);
		Bullet* bullet = new Bullet(enemy->GetBullet()->GetID());
		if (enemy->GetBullet()->GetID() == CATEGORY_FOLLOW_GUN) {
			bullet->InitA(enemy->GetBullet()->GetAttackDame(), enemy->GetBullet()->GetAttackSpeed(), dir*enemy->GetBullet()->GetSpeedOfBullet().x, dir*scale*enemy->GetBullet()->GetSpeedOfBullet().x, enemy->GetBullet()->GetMaxOfLength());
		}
		else {
			bullet->InitA(enemy->GetBullet()->GetAttackDame(), enemy->GetBullet()->GetAttackSpeed(), dir*enemy->GetBullet()->GetSpeedOfBullet().x, enemy->GetBullet()->GetSpeedOfBullet().y, enemy->GetBullet()->GetMaxOfLength());
			if(enemy->GetBullet()->GetID() == CATEGORY_ENEMY_GUN) posBullet = Vector3(posEnemy.x, posEnemy.y+ box.y, 0);
		}

		bullet->SetIsChange();
		bullet->setModel(enemy->GetBullet()->getModel());
		bullet->setShader(enemy->GetBullet()->getShaders());
		bullet->SetTexture(enemy->GetBullet()->getTexture());
		bullet->SetPosition(posBullet);
		bullet->SetScale(enemy->GetBullet()->GetScale());
		bullet->SetRotation(enemy->GetBullet()->GetRotation());
		bullet->InitWVP();
		bullet->SetBodyObject(posBullet.x, posBullet.y, m_world, false);
		bullet->m_current_anim = Idle * enemy->m_direction;

		AddBullet(bullet);
	}
}

void SceneManager::BossAttack() {
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	b2Vec2 posBoss = m_boss->getBody()->GetPosition();
	float dir = posMainCharacter.x > posBoss.x ? 1.0f : -1.0f;
	if (m_boss->GetBullet()->GetID() == CATEGORY_BOSS_GUN) {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hasaki2.mp3", false);
		
		Bullet* bullet = new Bullet(m_boss->GetBullet()->GetID());
		Vector3 posBullet = Vector3(posBoss.x, posBoss.y, 0);
		float scale = (posBoss.y - posMainCharacter.y) / (posBoss.x - posMainCharacter.x);

		if (m_boss->GetNumOfBullet() == 3) {
			bullet->InitA(m_boss->GetBullet()->GetAttackDame(), m_boss->GetBullet()->GetAttackSpeed(), dir*m_boss->GetBullet()->GetSpeedOfBullet().x, m_boss->GetBullet()->GetSpeedOfBullet().y, m_boss->GetBullet()->GetMaxOfLength());
		}
		else {
			bullet->InitA(m_boss->GetBullet()->GetAttackDame(), m_boss->GetBullet()->GetAttackSpeed(), dir*m_boss->GetBullet()->GetSpeedOfBullet().x, dir*scale*m_boss->GetBullet()->GetSpeedOfBullet().x, m_boss->GetBullet()->GetMaxOfLength());
		}
		bullet->SetIsChange();
		bullet->setModel(m_boss->GetBullet()->getModel());
		bullet->setShader(m_boss->GetBullet()->getShaders());
		bullet->SetTexture(m_boss->GetBullet()->getTexture());
		bullet->SetPosition(posBullet);
		bullet->SetScale(m_boss->GetBullet()->GetScale());
		bullet->SetRotation(m_boss->GetBullet()->GetRotation());
		bullet->InitWVP();
		bullet->SetBodyObject(posBullet.x, posBullet.y, m_world, false, false);
		bullet->m_current_anim = Idle * dir;

		AddBullet(bullet);
	}
	else {
		ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hasaki1.mp3", false);
		for (int i = 0; i < m_boss->GetNumOfBullet(); ++i) {
			Bullet* bullet = new Bullet(m_boss->GetBullet()->GetID());
			Vector3 posBullet;
			if (m_boss->GetBullet()->GetID() == CATEGORY_RADIATE_GUN) {
				posBullet = Vector3(posBoss.x, posBoss.y, 0);
			}
			else {
				float widthboss = m_boss->getBody()->GetFixtureList()->GetAABB(0).GetExtents().x * 2 / m_boss->GetNumOfBullet();
				posBullet = Vector3(posBoss.x + widthboss, posBoss.y + widthboss *(i - int(m_boss->GetNumOfBullet() / 2)), 0);
			}

			float scale = (posBullet.y - posMainCharacter.y) / (posBullet.x - posMainCharacter.x);

			if (m_boss->GetBullet()->GetID() == CATEGORY_FOLLOW_GUN) {
				bullet->InitA(m_boss->GetBullet()->GetAttackDame(), m_boss->GetBullet()->GetAttackSpeed(), dir*m_boss->GetBullet()->GetSpeedOfBullet().x, dir*scale*m_boss->GetBullet()->GetSpeedOfBullet().x, m_boss->GetBullet()->GetMaxOfLength());
			}
			else if (m_boss->GetBullet()->GetID() == CATEGORY_RADIATE_GUN) {
				bullet->InitA(m_boss->GetBullet()->GetAttackDame(), m_boss->GetBullet()->GetAttackSpeed(), dir*m_boss->GetBullet()->GetSpeedOfBullet().x, dir*m_boss->GetBullet()->GetSpeedOfBullet().x / m_boss->GetNumOfBullet() * (i - int(m_boss->GetNumOfBullet() / 2)), m_boss->GetBullet()->GetMaxOfLength());
			}
			else {
				bullet->InitA(m_boss->GetBullet()->GetAttackDame(), m_boss->GetBullet()->GetAttackSpeed(), dir*m_boss->GetBullet()->GetSpeedOfBullet().x, m_boss->GetBullet()->GetSpeedOfBullet().y, m_boss->GetBullet()->GetMaxOfLength());
			}
			bullet->SetIsChange();
			bullet->setModel(m_boss->GetBullet()->getModel());
			bullet->setShader(m_boss->GetBullet()->getShaders());
			bullet->SetTexture(m_boss->GetBullet()->getTexture());
			bullet->SetPosition(posBullet);
			bullet->SetScale(m_boss->GetBullet()->GetScale());
			bullet->SetRotation(m_boss->GetBullet()->GetRotation());
			bullet->InitWVP();
			bullet->SetBodyObject(posBullet.x, posBullet.y, m_world, false);
			bullet->m_current_anim = Idle * dir;

			AddBullet(bullet);
		}
	}
}

void SceneManager::ChangeGun(bool isEmptyBullet) {
	m_time = 0;
	Bullet *bullet = m_ListGunOfPlayer[0];
	if (isEmptyBullet) {
		bullet->ResetBullet();
		m_ListGunOfPlayer.erase(m_ListGunOfPlayer.begin());
		m_ListGunOfPlayer.push_back(bullet);
	}
	else {
		m_ListGunOfPlayer[0] = m_ListGunOfPlayer[1];
		m_ListGunOfPlayer[1] = bullet;
	}
}

void SceneManager::SetStateHellGun(Bullet* hellBullet, float enemyWidth) {
	float v = hellBullet->GetSpeedOfBullet().x > 0 ? 1.0f : -1.0f;
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hellgun.wav", false);

	for (int i = 0; i < 3; ++i) {
		b2Vec2 posHellBullet = hellBullet->getBody()->GetPosition();
		Bullet* bullet = new Bullet(hellBullet->GetID());
		bullet->InitA(hellBullet->GetAttackDame(), hellBullet->GetAttackSpeed(), hellBullet->GetSpeedOfBullet().x, hellBullet->GetSpeedOfBullet().x / 2 * (i - 1), hellBullet->GetMaxOfLength());
		Vector3 posBullet = Vector3(posHellBullet.x + v * (hellBullet->GetBox().x * 3 + 2 * enemyWidth), posHellBullet.y, 0);
		bullet->setModel(hellBullet->getModel());
		bullet->setShader(hellBullet->getShaders());
		bullet->SetTexture(hellBullet->getTexture());
		bullet->SetPosition(posBullet);
		bullet->SetScale(hellBullet->GetScale());
		bullet->SetRotation(hellBullet->GetRotation());
		bullet->InitWVP();
		bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
		bullet->m_current_anim = 2*m_direction;

		AddBullet(bullet);
	}
}
float prev = 0, now = 0;
void SceneManager::Update(float deltaTime) {
	++Camera::GetInstance()->m_iUpdateFase;
	if (m_bChangeScreen) { //Check if Change Screen
		timeCount += deltaTime;
		if (timeCount > 2.5f) {
			ChangeToResultScreen(m_bIsVictory);
			return; // m_bIsVictory default = false;
		}
	}
	if (m_MainCharacter->isDie()) {
		m_MainCharacter->playDead(deltaTime);
		Singleton<GameplayUI>::GetInstance()->Update(deltaTime);
		for (int i = 0; i < m_listEnemyInWorld.size(); ++i) m_listEnemyInWorld[i]->getBody()->SetEnabled(false);
		int lop = deltaTime / 0.003;
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(0, 20000), true);
		for (int i = 0; i < lop; ++i) {
			m_world->Step(0.07f, 6, 2);
			m_MainCharacter->Update(deltaTime);
		}
		if (m_MainCharacter->getDead()) {
			m_bChangeScreen = true;
			m_bIsVictory = false;
		}
		return;
	}
	//Set GameplayUI
	Singleton<GameplayUI>::GetInstance()->SetNumberOfBullets(m_ListGunOfPlayer[0]->GetNumberOfBullet(), m_ListGunOfPlayer[1]->GetNumberOfBullet());
	Singleton<GameplayUI>::GetInstance()->Update(deltaTime);

	if (Camera::GetInstance()->is_wound) ++cnt;
	if (cnt > 31) {
		cnt = 0;
		Camera::GetInstance()->is_wound = false;
	}
	 b2Vec2 pos = m_MainCharacter->getBody()->GetPosition();
	 Camera::GetInstance()->Update(deltaTime, pos.x, pos.y, m_direction);
	 for (int i = 0; i < m_ListBackground.size(); ++i) m_ListBackground[i]->Update(deltaTime);


	 int col = Globals::screenWidth / WIDTH * 2 + 1;
	 int row = Globals::screenHeight / WIDTH * 2 + 1;
	 int w = pos.x / WIDTH + m_listTerrain[0].size() / 2;
	 int h = pos.y / WIDTH + m_listTerrain.size() / 2;

	wlow = w - col > 0 ? w - col : 0;
	whigh = w + col < (int)m_listTerrain[0].size() ? w + col : (int)m_listTerrain[0].size();
	hlow = h - row > 0 ? h - row : 0;
	hhigh = h + row < (int)m_listTerrain.size() ? h + row : (int)m_listTerrain.size();

	for (int i = hlow; i < hhigh; ++i) {
		for (int j = wlow; j < whigh; ++j) {
			if(m_listTerrain[i][j] != NULL) m_listTerrain[i][j]->SetBodyObject(WIDTH, WIDTH ,m_world);
			if (mapEnemy[{i, j}] > 0) {
				int id = mapEnemy[{i, j}] - 1;
				int left = mapLimit[{i, j}].first;
				int right = mapLimit[{i, j}].second;
				int r = m_listTerrain.size();
				int c = m_listTerrain[0].size();
				Enemy* enemy = new Enemy(id);
				Model * emodel = new Model(m_listEnemy[id]->getModel());
				enemy->setModel(emodel);
				enemy->setShader(m_listEnemy[id]->getShaders());
				enemy->SetTexture(m_listEnemy[id]->getTexture());
				enemy->SetBulletID(m_listEnemy[id]->GetBulletID());
				enemy->SetPosition(WIDTH*(j - c / 2), WIDTH*(i - r / 2), 0.0f);
				enemy->SetLimit(WIDTH*(left - c / 2), WIDTH*(right - c / 2));
				enemy->SetScale(m_listEnemy[id]->GetScale());
				enemy->SetRotation(m_listEnemy[id]->GetRotation());
				enemy->SetMaxHP(m_listEnemy[id]->GetHP());
				enemy->SetSpeed(m_listEnemy[id]->GetSpeed().x, m_listEnemy[id]->GetSpeed().y);
				Vector2 box = m_listEnemy[id]->getTransBox();
				enemy->setTransBox(box.x, box.y);
				enemy->InitWVP();
				for (int k = 0; k < (int)m_ListGunOfEnemy.size(); k++) {
					if (m_ListGunOfEnemy[k]->GetID() == m_listEnemy[id]->GetBulletID()) {
						enemy->SetBullet(m_ListGunOfEnemy[k]);
						break;
					}
				}
				// set HP texture
				Model* modelHP = new Model();
				modelHP->InitSprite(0, 0, 180, 18, 180, 18, false);
				m_ModelDump.push_back(modelHP);
				Healthy* whiteHp = new Healthy(0);
				whiteHp->setModel(modelHP);
				whiteHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
				whiteHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(WHITE));
				whiteHp->SetPosition(enemy->GetPosition());
				whiteHp->SetOriginScale(1, 1, 1);
				whiteHp->SetRotation(enemy->GetRotation());
				whiteHp->InitWVP();
				Healthy* redHp = new Healthy(1);
				redHp->setModel(modelHP);
				redHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
				redHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(RED));
				redHp->SetPosition(enemy->GetPosition());
				redHp->SetOriginScale(1, 1, 1);
				redHp->SetRotation(enemy->GetRotation());
				redHp->InitWVP();

				m_ObjectDump.push_back(redHp);
				m_ObjectDump.push_back(whiteHp);
				enemy->SetHPTexture(whiteHp);
				enemy->SetHPTexture(redHp, false);
				enemy->SetBodyObject(m_world);
				AddEnemy(enemy);
				mapEnemy[{i, j}] = 0;
			}
		}
	}

	if (m_IsBossAppear == true && m_boss != NULL) {
		if (m_boss->IsMove()) {
			m_boss->UploadSpeed();
			m_boss->Update(deltaTime);
		}
		else {
			m_boss->Update(deltaTime);
			m_boss->UpdateAttack(deltaTime);
			if (m_boss->isAttack()) {
				BossAttack();
				m_boss->UploadNum();
				m_boss->SwapGun();
			}
		}
	}

	for (int i = 0; i < (int)m_listBulletInWorld.size(); ++i) {
		if (m_listBulletInWorld[i]->checkDraw() == false) {
			RemoveBullet(i);
		}
		else m_listBulletInWorld[i]->UpdateAnimation(deltaTime);
	}

	// set key
	m_time += deltaTime;
	m_timeChangeGun += deltaTime;
	m_time_roll += deltaTime;
	m_timeHurt += deltaTime;
	CheckMovement();

	for (int i = 0; i < m_listEnemyInWorld.size(); ++i) {
		if (m_listEnemyInWorld[i]->checkDraw()) {
			m_listEnemyInWorld[i]->m_direction = (m_listEnemyInWorld[i]->GetPosition().x < m_MainCharacter->GetPosition().x) ? 1 : -1;
			m_listEnemyInWorld[i]->UpdateAnimation(deltaTime);
		}
		if ((m_listEnemyInWorld[i]->GetBulletID() >= 0 && enemySeen(m_listEnemyInWorld[i])) || m_listEnemyInWorld[i]->GetID() == 4) {
			m_listEnemyInWorld[i]->UpdateAttack(deltaTime);
			if (m_listEnemyInWorld[i]->isAttack()) {
				m_listEnemyInWorld[i]->cnt++;
				m_listEnemyInWorld[i]->m_current_anim = 2;
				if (m_listEnemyInWorld[i]->cnt > 25) {
					EnemyAttack(m_listEnemyInWorld[i]);
					m_listEnemyInWorld[i]->m_time = 0;
					m_listEnemyInWorld[i]->cnt = 0;
				}
			}
			else m_listEnemyInWorld[i]->m_current_anim = 1;
		}
		else m_listEnemyInWorld[i]->m_current_anim = 1;
	}

	for (int i = 0; i < m_listEnemyDead.size(); ++i) {
		m_listEnemyDead[i]->playDead(deltaTime);
		if (m_listEnemyDead[i]->getDead()) {
			delete m_listEnemyDead[i]->getModel();
			delete m_listEnemyDead[i];
			m_listEnemyDead.erase(m_listEnemyDead.begin() + i);
		}
	}

	// set update
	mainIcon->UpdateAnimation(deltaTime);
	m_MainCharacter->getBody()->SetFixedRotation(true);
	m_MainCharacter->UpdateAnimation(deltaTime);
	m_boss->m_direction = (m_boss->GetPosition().x < m_MainCharacter->GetPosition().x) ? 1 : -1;
	m_boss->m_current_anim = Idle * m_boss->m_direction;
	m_boss->UpdateAnimation(deltaTime);
	// set v
	static float impulse = m_MainCharacter->getBody()->GetMass() * 40;;
	float impulseX = m_Horizontal * 900;

	if (jumpstep > 0) {
		if (m_MainCharacter->m_current_anim == RunJump * m_direction) impulseX = 40.0f*m_direction*900.0f;
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(impulseX, -impulse), true);
		--jumpstep;
	}
	else {
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(impulseX, impulse), true);
	}

	static int32 velocityIterations = 6;
	static int32 positionIterations = 2;
	int lop = deltaTime / 0.003f;
	float st = 0.35f / (float) lop;

	for(int ilop = 0;ilop < lop;++ilop){
		m_world->Step(st, velocityIterations, positionIterations);
		m_MainCharacter->Update(deltaTime);
		now = m_MainCharacter->GetPosition().y;

		// Main's collision
		is_in_ground = false;
		for (b2ContactEdge * edge = m_MainCharacter->getBody()->GetContactList(); edge != NULL; edge = edge->next) {
			b2Fixture * a = edge->contact->GetFixtureA();
			b2Fixture * b = edge->contact->GetFixtureB();

			// check terrain collision
			if (b->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
				if (m_MainCharacter->getBody()->GetPosition().y < b->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
					}
				}
			}
			else if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
				if (m_MainCharacter->getBody()->GetPosition().y < a->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
					}
				}
			}
			else if (b->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
				if (m_MainCharacter->getBody()->GetPosition().y < b->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
					}
					if (a->GetFilterData().groupIndex == -1) {
						b2Vec2 vel = m_MainCharacter->getBody()->GetLinearVelocity();
						m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(vel.x * 0.5, vel.y));
					}
				}
			}
			else if (a->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
				if (m_MainCharacter->getBody()->GetPosition().y < a->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
					}
					if (b->GetFilterData().groupIndex == -1) {
						b2Vec2 vel = m_MainCharacter->getBody()->GetLinearVelocity();
						m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(vel.x * 0.5, vel.y));
					}
				}
			} // end terrain

			// check bullet collision
			else if (a->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || b->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || a->GetFilterData().categoryBits == CATEGORY_BULLET_BOSS || b->GetFilterData().categoryBits == CATEGORY_BULLET_BOSS) {
				Bullet * bullet;
				if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - b->GetDensity());
					bullet = reinterpret_cast<Bullet*>(b->GetUserData().pointer);
				}
				else {
					bullet = reinterpret_cast<Bullet*>(b->GetUserData().pointer);
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - a->GetDensity());
				}
				ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hurt2.wav", false);
				Camera::GetInstance()->is_wound = true;
				bullet->m_bRemoveAble = true;
			} // end bullet

			// check boss collision
			else if (a->GetFilterData().categoryBits == CATEGORY_BOSS || b->GetFilterData().categoryBits == CATEGORY_BOSS) {
				m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 4);
				Camera::GetInstance()->is_wound = true;
				if (m_timeHurt >= 0.5) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hurt2.wav", false);
					m_timeHurt = 0;
				}
			} // end check boss

			// check enemy collision
			else if (b->GetFilterData().categoryBits == CATEGORY_ENEMY) {
				Enemy * enemy = reinterpret_cast<Enemy*> (b->GetUserData().pointer);
				if (enemy->GetBulletID() < 0) enemy->m_current_anim = 2;
				m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 1);
				Camera::GetInstance()->is_wound = true;
				if (m_timeHurt >= 0.5) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hurt2.wav", false);
					m_timeHurt = 0;
				}
			}
			else if (a->GetFilterData().categoryBits == CATEGORY_ENEMY) {
				Enemy * enemy = reinterpret_cast<Enemy*> (a->GetUserData().pointer);
				if (enemy->GetBulletID() < 0) enemy->m_current_anim = 2;
				m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 1);
				Camera::GetInstance()->is_wound = true;
				if (m_timeHurt >= 0.5) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/hurt2.wav", false);
					m_timeHurt = 0;
				}
			}// end check enemy

		}
		prev = now;
		if (is_in_ground) numJump = 0;
		// end Main


		// Boss's collision
		if (m_IsBossAppear && m_boss != NULL) {
			m_boss->Update(deltaTime);
			for (b2ContactEdge* edge = m_boss->getBody()->GetContactList(); edge != NULL; edge = edge->next) {
				b2Fixture * a = edge->contact->GetFixtureA();
				b2Fixture * b = edge->contact->GetFixtureB();
				if (b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_boss->SetHP(m_boss->GetHP() - b->GetDensity());
				}
				else if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_boss->SetHP(m_boss->GetHP() - a->GetDensity());
				}
			}
			if (m_boss->isDie()) {
				m_IsBossAppear = false;
				m_world->DestroyBody(m_boss->getBody());
				break;
			}
		}
		// end Boss

		// Enemy's collision
		float d;
		for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
			if (m_listEnemyInWorld[i]->GetBulletID() < 0) {
				d = m_MainCharacter->GetPosition().x > m_listEnemyInWorld[i]->GetPosition().x ? 1 : -1;
				
				b2Vec2 pos = m_listEnemyInWorld[i]->getBody()->GetPosition();
				if (enemySeen(m_listEnemyInWorld[i])) {
					if(m_listEnemyInWorld[i]->m_bFollowing) m_listEnemyInWorld[i]->getBody()->SetLinearVelocity(b2Vec2(2*d* m_listEnemyInWorld[i]->GetSpeed().x, m_listEnemyInWorld[i]->GetSpeed().y));
					else if (m_listEnemyInWorld[i]->checkRect(m_MainCharacter->GetPosition().x)) {
						m_listEnemyInWorld[i]->m_bFollowing = true;
					}
				}
				else m_listEnemyInWorld[i]->m_bFollowing = false;
			}
			m_listEnemyInWorld[i]->Update(deltaTime);
			for (b2ContactEdge* edge = m_listEnemyInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				Bullet * bullet;
				if (b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_listEnemyInWorld[i]->SetHP(m_listEnemyInWorld[i]->GetHP() - b->GetDensity());
					bullet = reinterpret_cast<Bullet*> (b->GetUserData().pointer);
					if (bullet->GetID() == CATEGORY_HELL_GUN) {
						if (bullet->IsChange()) {
							SetStateHellGun(bullet, a->GetAABB(0).GetExtents().x);
						}
						bullet->m_bRemoveAble = true;
					}
					else if (bullet->GetID() == CATEGORY_BOOMERANG) {
						ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boomerang.wav", false);
						b->SetFilterData(filterBoomerang1);
						m_MainCharacter->SetHP(m_MainCharacter->GetHP() + bullet->GetAttackDame() * 0.2);
						float vBullet = bullet->GetSpeedOfBullet().x;
						float vMoster = a->GetBody()->GetLinearVelocity().x;
						float pBullet = bullet->GetPosition().x;
						float pMoster = a->GetBody()->GetPosition().x;
						float plength = pMoster - pBullet;
						float v = bullet->GetSpeedOfBullet().x > 0 ? 1 : -1;
						float length = a->GetAABB(0).GetExtents().x + v * plength + bullet->GetBox().x;
						length = vBullet * length / (vBullet - vMoster);
						length = length > 0 ? length : -length;
						bullet->SetLengthBoomerang(length);
						bullet->SetOldPos(bullet->GetPosition().x);
						bullet->getBody()->GetFixtureList()->SetFilterData(filterBoomerang2);
					}
					else bullet->m_bRemoveAble = true;
				}
				else if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_listEnemyInWorld[i]->SetHP(m_listEnemyInWorld[i]->GetHP() - a->GetDensity());
					bullet = reinterpret_cast<Bullet*> (a->GetUserData().pointer);
					if (bullet->GetID() == CATEGORY_HELL_GUN) {
						if (bullet->IsChange()) {
							SetStateHellGun(bullet, b->GetAABB(0).GetExtents().x);
						}
						bullet->m_bRemoveAble = true;
					}
					else if (bullet->GetID() == CATEGORY_BOOMERANG) {
						ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boomerang.wav", false);
						a->SetFilterData(filterBoomerang1);
						m_MainCharacter->SetHP(m_MainCharacter->GetHP() + bullet->GetAttackDame() * 0.2);
						float vBullet = bullet->GetSpeedOfBullet().x;
						float vMoster = b->GetBody()->GetLinearVelocity().x;
						float pBullet = bullet->GetPosition().x;
						float pMoster = b->GetBody()->GetPosition().x;
						float plength = pMoster - pBullet;
						float v = bullet->GetSpeedOfBullet().x > 0 ? 1 : -1;
						float length = b->GetAABB(0).GetExtents().x + v * plength + bullet->GetBox().x;
						length = vBullet * length / (vBullet - vMoster);
						length = length > 0 ? length : -length;
						bullet->SetLengthBoomerang(length);
						bullet->SetOldPos(bullet->GetPosition().x);
						bullet->getBody()->GetFixtureList()->SetFilterData(filterBoomerang2);
					}
					else bullet->m_bRemoveAble = true;
				}
			}
			if (m_listEnemyInWorld[i]->isDie()) {
				if (m_listEnemyInWorld[i]->GetID() == 4) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/tower.wav", false);
					m_IsTowerDefend = true;
				}
				else if (m_listEnemyInWorld[i]->GetID() == 0) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bigdragon.wav", false);
				}
				else if (m_listEnemyInWorld[i]->GetID() == 1) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/moster.wav", false);
				}
				else if (m_listEnemyInWorld[i]->GetID() == 2) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/medusa.wav", false);
				}
				else if (m_listEnemyInWorld[i]->GetID() == 3) {
					ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/smalldragon.wav", false);
				}

				/*for (int j = 0; j < (int)m_listBulletInWorld.size(); ++j) {
					if (m_listBulletInWorld[j]->GetTarget() == m_listEnemyInWorld[i]->getBody()) {
						m_listBulletInWorld[j]->SetTarget(NULL);
					}
				}*/
				m_world->DestroyBody(m_listEnemyInWorld[i]->getBody());
				m_listEnemyDead.push_back(m_listEnemyInWorld[i]);
				m_listEnemyInWorld.erase(m_listEnemyInWorld.begin() + i);
				i--;
			}
		}
		// end Enemy

		// Bullet's collision
		for (int i = 0; i < (int)m_listBulletInWorld.size(); ++i) {
			if (m_listBulletInWorld[i]->m_bRemoveAble) {
				RemoveBullet(i);
				i--;
			}
			else {
				m_listBulletInWorld[i]->Update(deltaTime);
				if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
					if (m_listBulletInWorld[i]->getBody()->GetFixtureList()->GetFilterData().groupIndex == -2) {
						float v = m_listBulletInWorld[i]->GetSpeedOfBullet().x > 0 ? 1 : -1;
						float length = m_listBulletInWorld[i]->GetPosition().x - m_listBulletInWorld[i]->GetOldPos();
						if (length * v > m_listBulletInWorld[i]->GetLengthBoomerang()) {
							m_listBulletInWorld[i]->getBody()->GetFixtureList()->SetFilterData(filterBoomerang1);
						}
					}
				}
				bool isContact = false;
				for (b2ContactEdge* edge = m_listBulletInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
					b2Fixture* a = edge->contact->GetFixtureA();
					b2Fixture* b = edge->contact->GetFixtureB();
					if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN || b->GetFilterData().categoryBits == CATEGORY_TERRAIN || a->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP || b->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
							if (m_listBulletInWorld[i]->IsChange()) {
								m_boomerang = m_listBulletInWorld[i]->getBody()->GetPosition().x;
								ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boomerang.wav", false);
								m_listBulletInWorld[i]->ReverseV();
								m_listBulletInWorld[i]->getBody()->GetFixtureList()->SetFilterData(filterBoomerang1);
								m_listBulletInWorld[i]->SetCurrLength(m_listBulletInWorld[i]->GetMaxOfLength() - m_listBulletInWorld[i]->GetCurrLength());
								break;
							}
							else {
								if (m_boomerang - m_listBulletInWorld[i]->getBody()->GetPosition().x > 400 || m_boomerang - m_listBulletInWorld[i]->getBody()->GetPosition().x < -400) {
									RemoveBullet(i);
									isContact = true;
									i--;
									break;
								}
							}
						}
						else {
							RemoveBullet(i);
							isContact = true;
							i--;
							break;
						}
					}
				}
				if (!isContact) {
					if (m_listBulletInWorld[i]->IsOverLength()) {
						RemoveBullet(i);
						i--;
					}
					else if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
						if (m_listBulletInWorld[i]->GetCurrLength() * 2 > m_listBulletInWorld[i]->GetMaxOfLength()) {
							if (m_listBulletInWorld[i]->IsChange()) {
								ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boomerang.wav", false);
								m_listBulletInWorld[i]->ReverseV();
								m_listBulletInWorld[i]->getBody()->GetFixtureList()->SetFilterData(filterBoomerang1);
							}
						}
					}
				}
			}
		}
	}

	if (m_currentLevel == 4){
		if(pos.x > 2400 && pos.x < 2800 && pos.y > -1400 && pos.y < -400){
			if (m_IsBossAppear == false && m_IsTowerDefend == true) {
				if (m_boss) {
					if (!m_boss->isDie()) {
						m_IsBossAppear = true;

						ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/boss.mp3", false);

						m_boss->SetBodyObject(m_boss->GetPosition().x, m_boss->GetPosition().y, m_world);

						// set HP texture
						Model* modelHP = new Model();
						modelHP->InitSprite(0, 0, 1000, 50, 1000, 50, false);
						m_ModelDump.push_back(modelHP);
						Healthy* whiteHp = new Healthy(0);
						whiteHp->setModel(modelHP);
						whiteHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
						whiteHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(WHITE));
						whiteHp->SetPosition(m_boss->GetPosition());
						whiteHp->SetOriginScale(1, 1, 1);
						whiteHp->SetRotation(m_boss->GetRotation());
						whiteHp->InitWVP();
						Healthy* redHp = new Healthy(1);
						redHp->setModel(modelHP);
						redHp->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
						redHp->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(RED));
						redHp->SetPosition(m_boss->GetPosition());
						redHp->SetOriginScale(1, 1, 1);
						redHp->SetRotation(m_boss->GetRotation());
						redHp->InitWVP();

						m_ObjectDump.push_back(redHp);
						m_ObjectDump.push_back(whiteHp);
						m_boss->SetHPTexture(whiteHp);
						m_boss->SetHPTexture(redHp, false);

						// set icon boss
						Model* modelIcon = new Model();
						modelIcon->InitSprite(0, 0, 150, 100, 150, 100, false);
						m_ModelDump.push_back(modelIcon);
						Healthy* icon = new Healthy(2);
						m_ObjectDump.push_back(icon);
						icon->setModel(modelIcon);
						icon->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
						icon->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(BOSS_ICON));
						icon->SetPosition(m_boss->GetPosition());
						icon->SetOriginScale(1, 1, 1);
						icon->SetRotation(m_boss->GetRotation());
						icon->InitWVP();

						m_boss->SetIcon(icon);
						m_boss->SetTarget(m_MainCharacter->getBody());
					}
				}
			}
		}
	}

	// victory
	if (m_IsTowerDefend == true) {
		if (m_currentLevel < 4) {
			m_bIsVictory = true;
			m_bChangeScreen = true;
			for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
				m_listEnemyInWorld[i]->getBody()->SetEnabled(false);
			}
		}
		else {
			if (m_boss->isDie()) {
				m_bIsVictory = true;
				m_bChangeScreen = true;
				for (int i = 0; i < (int)m_listEnemyInWorld.size(); ++i) {
					m_listEnemyInWorld[i]->getBody()->SetEnabled(false);
				}
			}
		}
	}
	if (m_MainCharacter->isDie()) return;
	for (int i = hlow; i < hhigh; ++i) {
		for (int j = wlow; j < whigh; ++j) {
			if (m_listTerrain[i][j] != NULL) {
				m_world->DestroyBody(m_listTerrain[i][j]->getBody());
			}
		}
	}
}

void SceneManager::Key(unsigned char key, bool isPressed) {
	if (isPressed) {
		switch (key)
		{
		case KEY_LEFT:
		case KEY_LEFT + 32:
			m_direction = -1;
			keyPressed = keyPressed | MOVE_LEFT;
			break;
		case KEY_RIGHT:
		case KEY_RIGHT + 32:
			m_direction = 1;
			keyPressed = keyPressed | MOVE_RIGHT;
			break;
		case KEY_JUMP:
		case KEY_JUMP + 32:
		case KEY_JUMP2:
			if (numJump < 2 && !(keyPressed & MOVE_JUMP)) {
				jumpstep = 30;
				m_MainCharacter->resetAnimation(RunJump);
				m_MainCharacter->resetAnimation(Jump);
				++numJump;
				keyPressed = keyPressed | MOVE_JUMP;
				ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/jump2.wav", false);
			}
			break;
		case KEY_CHANGE_GUN:
		case KEY_CHANGE_GUN + 32:
			keyPressed = keyPressed | CHANGE_GUN;
			break;
		case KEY_SHOOT:
		case KEY_SHOOT + 32:
			keyPressed = keyPressed | SHOOT;
			break;
		case VK_SHIFT:
			if (keyPressed & ROLL) break;
			if (!is_roll && m_time_roll > 0.5f) {
				roll_step = 30;
				m_MainCharacter->resetAnimation(RunJump);
				m_MainCharacter->resetAnimation(Idle);
				m_MainCharacter->resetAnimation(Falling);
				is_roll = true;
				keyPressed = keyPressed | ROLL;
			}
			break;
		}
	}
	else {
		m_MainCharacter->resetAnimation(Run);
		m_MainCharacter->resetAnimation(Idle);
		m_MainCharacter->resetAnimation(Falling);
		switch (key)
		{
		case KEY_LEFT:
		case KEY_LEFT + 32:
			keyPressed = keyPressed ^ MOVE_LEFT;
			m_Horizontal = 0.0f;
			break;
		case KEY_RIGHT:
		case KEY_RIGHT + 32:
			keyPressed = keyPressed ^ MOVE_RIGHT;
			m_Horizontal = 0.0f;
			break;
		case KEY_JUMP:
		case KEY_JUMP + 32:
		case KEY_JUMP2:
			keyPressed = keyPressed ^ MOVE_JUMP;
			break;
		case KEY_CHANGE_GUN:
		case KEY_CHANGE_GUN + 32:
			keyPressed = keyPressed ^ CHANGE_GUN;
			m_changeGun = 0;
			break;
		case KEY_SHOOT:
		case KEY_SHOOT + 32:
			keyPressed = keyPressed ^ SHOOT;
			m_shoot = 0;
			m_MainCharacter->resetGun();
			break;
		case VK_SHIFT:
			keyPressed = keyPressed ^ ROLL;
			break;
		case '1':
			Camera::GetInstance()->m_iOption = 1;
			break;
		case '2':
			Camera::GetInstance()->m_iOption = 2;
			break;
		case '3':
			Camera::GetInstance()->m_iOption = 3;
			break;
		case 'N':
			m_bChangeScreen = true;
			m_bIsVictory = true;
			//m_MainCharacter->getBody()->SetTransform(b2Vec2(0, 0), 0);
			break;
		}
	}
}

void SceneManager::CheckMovement() {
	if (!is_roll) {
		if (is_in_ground) {
			m_MainCharacter->resetAnimation(Falling);
			m_MainCharacter->m_current_anim = Idle * m_direction;
		}
		else if (jumpstep <= 0) {
			m_MainCharacter->m_current_anim = Falling * m_direction;
		}
		if (keyPressed & MOVE_RIGHT) {
			m_direction = 1;
			if (is_in_ground) m_MainCharacter->m_current_anim = Run;
			m_Horizontal = 40.0f;
		}
		else if (keyPressed & MOVE_LEFT) {
			m_direction = -1;
			if (is_in_ground) m_MainCharacter->m_current_anim = -Run;
			m_Horizontal = -40.0f;
		}

		if (keyPressed & MOVE_JUMP) {
			if (jumpstep > 0) {
				if (m_Horizontal == 0) m_MainCharacter->m_current_anim = Jump * m_direction;
				else {
					m_MainCharacter->m_current_anim = RunJump * m_direction;
				}
			}
		}


		if (keyPressed & SHOOT) {
			m_Horizontal = 0;
			if (m_time > 0.5f) m_MainCharacter->m_current_anim = m_ListGunOfPlayer[0]->GetID() * m_direction;
			m_shoot = 1.0f;
			if (Camera::GetInstance()->is_shoot == true) {
				Camera::GetInstance()->is_shoot = false;
				Shoot();
				if (m_ListGunOfPlayer[0]->IsEmptyBullet()) {
					ChangeGun(true);
					m_time = 0;
				}
			}
		}
		if (keyPressed & CHANGE_GUN) {
			m_changeGun = 1.0f;
			if (m_timeChangeGun >= 3.0) {
				ChangeGun(false);
				m_timeChangeGun = 0;
			}
		}
		if (Camera::GetInstance()->is_wound) {
			m_MainCharacter->m_current_anim = Wound * m_direction;
		}
		else m_MainCharacter->resetAnimation(Wound);
	}
	else {
		if (roll_step > 0) {
			m_MainCharacter->m_current_anim = RunJump * m_direction;
			m_MainCharacter->getBody()->GetFixtureList()->SetFilterData(filterRoll);
			m_time_roll = 0;
			m_MainCharacter->getBody()->ApplyLinearImpulse(b2Vec2(80000 * m_direction, 50000), m_MainCharacter->getBody()->GetWorldCenter(), true);
			--roll_step;
		}
		else {
			is_roll = false;
			m_MainCharacter->getBody()->GetFixtureList()->SetFilterData(filterMain);
		}
	}
}

bool SceneManager::enemySeen(Enemy * enemy) {
	b2Vec2 mpos = m_MainCharacter->getBody()->GetPosition();
	b2Vec2 epos = enemy->getBody()->GetPosition();
	if (enemy->GetBulletID() >= 0) {
		if (epos.x < mpos.x - 1280 || epos.x >(mpos.x + 1280)) return false;
		if (epos.y < mpos.y - 300 || epos.y > mpos.y + 300) return false;
	}
	else {
		if (epos.x < mpos.x - 1000 || epos.x > mpos.x + 1000) return false;
		if (epos.y < mpos.y - 30 || epos.y > mpos.y + 30) return false;
	}
	return true;
}
