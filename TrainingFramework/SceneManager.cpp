#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "define.h"
#include <iostream>
#include "Globals.h"
#include "GameplayUI.h"
#include "Healthy.h"


SceneManager::SceneManager()
{
	filterMain.categoryBits = CATEGORY_PLAYER;
	filterMain.maskBits = MASK_PLAYER;
	filterMain.groupIndex = -1;
	filterRoll.categoryBits = CATEGORY_PLAYER;
	filterRoll.maskBits = MASK_PLAYER;
	filterRoll.groupIndex = -2;
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
	keyPressed = 0;
	m_time_roll = 0;
	is_roll = false;
	roll_step = 0;
}

void SceneManager::ChangeToResultScreen(bool bIsVictory)
{
	GameStateMachine::GetInstance()->PushState(StateTypes::GS_RESULT, bIsVictory);
}

void SceneManager::Init() {
	numJump = 0;
	jumpstep = 0;
	is_in_ground = false;
	indices = new int[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	glGenBuffers(1, &Camera::GetInstance()->iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Camera::GetInstance()->iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
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
	ppmodel->InitSprite(0, 0, 50, 50, 200, 100);
	ppmodel->b_IsAnimation = true;
	ppmodel->addAnimation(ResourceManager::GetInstance()->GetAnimationAtID(28));
	mainIcon->setModel(ppmodel);
	mainIcon->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(31));
	mainIcon->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	mainIcon->SetPosition(0, 0, 0);
	mainIcon->SetScale(3, 3, 1);
	mainIcon->m_current_anim = 1;
	mainIcon->InitWVP();
	Singleton<GameplayUI>::GetInstance()->Init(); //Init GameplayUI
	Singleton<GameplayUI>::GetInstance()->SetMainCharacter(m_MainCharacter); //Set MainCharacter to show information's MC
}

void SceneManager::ReadFile(FILE* f_SM)
{
	//Cameras
	int numOfCameras;
	fscanf_s(f_SM, "#Cameras: %d\n", &numOfCameras);
	for (register int i = 0; i < numOfCameras; i++) {
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
	for (register int i = 0; i < numOfObjects; i++) {
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
		pModel->InitSprite(x, y, w, h, tw, th);
		int num_anim;
		fscanf_s(f_SM, "ANIMATIONS %d\n", &num_anim);
		if (num_anim > 0) {
			pModel->b_IsAnimation = true;
			for (int i = 0; i < num_anim; i++) {
				fscanf_s(f_SM, "ANIMATION %d\n", &anim);
				pModel->addAnimation(ResourceManager::GetInstance()->GetAnimationAtID(anim));
			}
		}
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
			modelHP->InitSprite(0, 0, 500, 30, 500, 30, false);
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

			m_MainCharacter->SetHPTexture(whiteHp);
			m_MainCharacter->SetHPTexture(redHp, false);
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
	fopen_s(&fp, "../map.txt", "r+");
	int ni; fscanf_s(fp, "%d\n", &ni);
	Vector4 * Omap = new Vector4[ni];
	for (int i = 0; i < ni; i++) {
		float x, y, w, h;
		fscanf_s(fp, "%f %f %f %f\n", &x, &y, &w, &h);
		Omap[i] = Vector4(x, y, w, h);
	}
	fclose(fp);
	char filetex[20];
	float tw = 0, th = 0;
	if (Camera::GetInstance()->i_state == 1) {
		strcpy_s(filetex, "../mapT2.tga");
		tw = 3200.0f; th = 400.0f;
	}
	if (Camera::GetInstance()->i_state == 2) {
		strcpy_s(filetex, "../mapT.tga");
		tw = 3200.0f;
		th = 1200.0f;
	}
	Texture * texx = new Texture(99, filetex);
	texx->Init();
	groundTest->setTexture(texx);
	groundTest->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	//
	int width, height, row, col, xi, num, index;

	fscanf_s(f_MAP, "%d %d\n", &width, &height);
	fscanf_s(f_MAP, "%d\n", &index);

	fscanf_s(f_MAP, "%d %d\n", &row, &col);

	for (int i = 0; i < row; i++) {
		std::vector<int> line;
		std::vector<int> isLine;
		std::vector<Terrain*> lineMap;
		for (int j = 0; j < col; j++) {
			fscanf_s(f_MAP, "%d ", &xi);
			line.push_back(xi);
			Terrain* terrain = NULL;
			if (xi >= 0) {
				terrain = new Terrain(xi);
				terrain->SetPosition(WIDTH *(j - col / 2), WIDTH*(i - row / 2), 0);
				if (xi <= 15) {
					terrain->SetBodyObject(WIDTH, WIDTH, m_world);
				}
				else if(xi <= 19){
					terrain->SetBodyObject(WIDTH, WIDTH, m_world, false);
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
	float scale = 1.5;
	num = (col * height) / (2 * scale * width * row) + 1;
	num = 2 * num + 1;
	int n = WIDTH * scale;
	Model* backgroundModel = new Model();
	backgroundModel->InitSprite(0, 0, (float)n * (width * row / height), (float)n * row, (float)n * (width * row / height), (float)n * row);

	for (int i = 0; i < num; i++) {
		Terrain *background = new Terrain(0);
		background->setModel(backgroundModel);
		background->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
		background->SetTexture(ResourceManager::GetInstance()->GetBackgroundAtID(index));
		background->SetPosition((float)n * (width * row / height) * (i - num / 2), -WIDTH * (scale - 1)/2 * row, 0);
		background->SetScale(Vector3(1, 1, 1));
		background->SetRotation(Vector3(0, 0, 0));
		background->InitWVP();
		m_ListBackground.push_back(background);
	}

	int numOfEnemy, id, posRow, posCol, left, right;
	fscanf_s(f_MAP, "#Enemy: %d\n", &numOfEnemy);
	for (int i = 0; i < numOfEnemy; i++) {
		fscanf_s(f_MAP, "%d %d %d %d %d\n", &id, &posRow, &posCol, &left, &right);
		mapEnemy[{posRow, posCol}] = id + 1;
		mapLimit[{posRow, posCol}] = { left, right };
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < (int)m_ListGunOfEnemy.size(); j++) {
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
	m_boss->SetTarget(m_MainCharacter->getBody());

	fclose(f_MAP);
}

int cnt = 0;
void SceneManager::Draw() {

	glUseProgram(ResourceManager::GetInstance()->GetShaderAtID(0)->program);

	for (int i = 0; i < (int)m_ListBackground.size(); i++) {
		m_ListBackground[i]->Draw();
	}
	groundTest->Draw();

	
	for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->Draw();
	}

	Vector3 pos = Camera::GetInstance()->GetPosition();
	m_ListGunOfPlayer[0]->SetPosition(pos.x - 1090, pos.y - 800, 0);
	m_ListGunOfPlayer[1]->SetPosition(pos.x - 1090, pos.y - 700, 0);
	mainIcon->SetPosition(pos.x - 1200, pos.y - 750, 0);
	mainIcon->Draw();
	for (int i = 0; i < 2; i++) {
		m_ListGunOfPlayer[i]->m_current_anim = 0;
		m_ListGunOfPlayer[i]->getModel()->resetTexture();
		m_ListGunOfPlayer[i]->Draw();
	}

	for (int i = 0; i < (int)m_listEnemyInWorld.size(); i++) {
		if (m_listEnemyInWorld[i]->checkDraw()) {
			m_listEnemyInWorld[i]->Draw();
			m_listEnemyInWorld[i]->DrawHP();
		}
	}
//	m_listEnemy[0]->Draw();
	if (m_IsBossAppear == true && m_boss != NULL) {
		m_boss->Draw();
		m_boss->DrawHP();
	}


	
	m_MainCharacter->Draw();
	m_MainCharacter->DrawHP();

	Singleton<GameplayUI>::GetInstance()->Draw(); //Draw GameplayUI
	if (Camera::GetInstance()->is_dead) {
		Singleton<GameplayUI>::GetInstance()->DrawGameOver();
	}
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
	Camera::GetInstance()->CleanUp();
	Singleton<GameplayUI>::GetInstance()->CleanUp();
	for (int i = 0; i < (int)m_listTerrain.size(); i++) {
		for (int j = 0; j < (int)m_listTerrain[i].size(); j++) {
			delete m_listTerrain[i][j];
		}
	}
	delete m_MainCharacter->getModel();
	delete m_MainCharacter;

	if (m_boss) {
		delete m_boss->getModel();
		delete m_boss;
	}
	for (int i = 0; i < (int)m_listEnemyInWorld.size(); i++) {
		delete m_listEnemyInWorld[i]->getModel();
		delete m_listEnemyInWorld[i];
	}

	for (int i = 0; i < (int)m_listEnemy.size(); i++) {
		delete m_listEnemy[i]->getModel();
		delete m_listEnemy[i];
	}

	for (int i = 0; i < (int)m_ListGunOfPlayer.size(); i++) {
		delete m_ListGunOfPlayer[i]->getModel();
		delete m_ListGunOfPlayer[i];
	}
	for (int i = 0; i < (int)m_ListGunOfEnemy.size(); i++) {
		delete m_ListGunOfEnemy[i]->getModel();
		delete m_ListGunOfEnemy[i];
	}
	delete mainIcon->getModel();
	delete mainIcon;
	delete groundTest;
	delete m_world;
}

void SceneManager::Shoot() {
	// set bullet
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	Bullet* bullet = new Bullet(m_ListGunOfPlayer[0]->GetID());
	bullet->InitA(m_ListGunOfPlayer[0]->GetAttackDame(), m_ListGunOfPlayer[0]->GetAttackSpeed(), m_direction*m_ListGunOfPlayer[0]->GetSpeedOfBullet().x, m_ListGunOfPlayer[0]->GetSpeedOfBullet().y, m_ListGunOfPlayer[0]->GetMaxOfLength());
	Vector3 posBullet = Vector3(posMainCharacter.x + m_direction * (m_MainCharacter->GetBox().x + m_ListGunOfPlayer[0]->GetBox().x / 2), posMainCharacter.y, 0);

	// set follow bazoka
	if (m_ListGunOfPlayer[0]->GetID() == CATEGORY_BAZOKA) {
		float minLength = 2000;
		for (int i = 0; i < (int)m_listEnemyInWorld.size(); i++) {
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
		for (int i = 0; i < 3; i++) {
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
		for (int i = 0; i < m_boss->GetNumOfBullet(); i++) {
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
	for (int i = 0; i < 3; i++) {
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
	//Set GameplayUI
	Singleton<GameplayUI>::GetInstance()->SetNumberOfBullets(m_ListGunOfPlayer[0]->GetNumberOfBullet(), m_ListGunOfPlayer[1]->GetNumberOfBullet());
	Singleton<GameplayUI>::GetInstance()->Update(deltaTime);

	if (Camera::GetInstance()->is_wound) ++cnt;
	if (cnt > 31) {
		cnt = 0;
		Camera::GetInstance()->is_wound = false;
	}
	static const double step = 1.0 / 70.0;
	 b2Vec2 pos = m_MainCharacter->getBody()->GetPosition();
	 Camera::GetInstance()->Update(deltaTime, pos.x, pos.y, m_direction);
	 int col = Globals::screenWidth / WIDTH * 2 + 1;
	 int row = Globals::screenHeight / WIDTH * 2 + 1;
	 int w = pos.x / WIDTH + m_listTerrain[0].size() / 2;
	 int h = 0 / WIDTH + m_listTerrain.size() / 2;
	wlow = w - col + 2 > 0 ? w - col + 2 : 0;
	whigh = w + col < (int)m_listTerrain[0].size() ? w + col : (int)m_listTerrain[0].size();
	hlow = h - row > 0 ? h - row : 0;
	hhigh = h + row < (int)m_listTerrain.size() ? h + row : (int)m_listTerrain.size();
	for (int i = hlow; i < hhigh; i++) {
		for (int j = wlow; j < whigh; j++) {
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

	for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
		if (m_listBulletInWorld[i]->checkDraw() == false) {
			RemoveBullet(i);
		}
		else m_listBulletInWorld[i]->UpdateAnimation(deltaTime);
	}

	// set key
	m_time += deltaTime;
	m_timeChangeGun += deltaTime;
	m_time_roll += deltaTime;
	CheckMovement();

	for (int i = 0; i < m_listEnemyInWorld.size(); i++) {
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
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(impulseX, -impulse), true);
		--jumpstep;
	}
	else {
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(impulseX, impulse), true);
	}

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	float lop = deltaTime / 0.003f;

	for(float i = 0;i < lop;i++){
		m_world->Step(0.07f, velocityIterations, positionIterations);
		m_MainCharacter->Update(deltaTime);
		now = m_MainCharacter->GetPosition().y;

		is_in_ground = false;
		for (b2ContactEdge* edge = m_MainCharacter->getBody()->GetContactList(); edge != NULL; edge = edge->next) {
			b2Fixture * a = edge->contact->GetFixtureA();
			b2Fixture * b = edge->contact->GetFixtureB();
			if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
				if (m_MainCharacter->getBody()->GetPosition().y < a->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
						numJump = 0;
					}
				}
			}
			if (b->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
				if (m_MainCharacter->getBody()->GetPosition().y < b->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
						numJump = 0;
					}
				}
			}
			if (a->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
				if (m_MainCharacter->getBody()->GetPosition().y < a->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
						numJump = 0;
					}
					if (b->GetFilterData().groupIndex == -1) {
						b2Vec2 vel = m_MainCharacter->getBody()->GetLinearVelocity();
						m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(vel.x * 0.5, vel.y));
					}
				}
			}
			if (b->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
				if (m_MainCharacter->getBody()->GetPosition().y < b->GetBody()->GetPosition().y) {
					if (now == prev) {
						is_in_ground = true;
						numJump = 0;
					}
				}
				if (a->GetFilterData().groupIndex == -1) {
					b2Vec2 vel = m_MainCharacter->getBody()->GetLinearVelocity();
					m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(vel.x * 0.5, vel.y));
				}
			}
		}


		if (m_IsBossAppear == true && m_boss != NULL) {
//			Singleton<GameplayUI>::GetInstance()->SetBoss(m_boss); //Set Boss to get info's Boss
			m_boss->Update(deltaTime);
			for (b2ContactEdge* edge = m_boss->getBody()->GetContactList(); edge != NULL; edge = edge->next) {
				b2Fixture * a = edge->contact->GetFixtureA();
				b2Fixture * b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 2);
					Camera::GetInstance()->is_wound = true;
//					break;
				}
				if (b->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 2);
					Camera::GetInstance()->is_wound = true;
//					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_boss->SetHP(m_boss->GetHP() - a->GetDensity());
				}
				if (b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_boss->SetHP(m_boss->GetHP() - b->GetDensity());
				}
			}
			if (m_boss->isDie()) {
				m_IsBossAppear = false;
//				Singleton<GameplayUI>::GetInstance()->SetBossAppear(m_bossAppear);
				m_world->DestroyBody(m_boss->getBody());
	//			m_boss = NULL;
				break;
			}
		}

		prev = now;

		for (int i = 0; i < (int)m_listEnemyInWorld.size(); i++) {
			m_listEnemyInWorld[i]->Update(deltaTime);
			for (b2ContactEdge* edge = m_listEnemyInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 0.2);
					Camera::GetInstance()->is_wound = true;
					break;
				}
				if (b->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - 0.2);
					Camera::GetInstance()->is_wound = true;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_listEnemyInWorld[i]->SetHP(m_listEnemyInWorld[i]->GetHP() - a->GetDensity());
				}
				if (b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					m_listEnemyInWorld[i]->SetHP(m_listEnemyInWorld[i]->GetHP() - b->GetDensity());
				}
				if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN || b->GetFilterData().categoryBits == CATEGORY_TERRAIN || a->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP || b->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
					m_listEnemyInWorld[i]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
				}
			}
			if (m_listEnemyInWorld[i]->isDie()) {
				if (m_listEnemyInWorld[i]->GetID() == 4) {
					m_IsTowerDefend = true;
				}

				for (int j = 0; j < (int)m_listBulletInWorld.size(); j++) {
					if (m_listBulletInWorld[j]->GetTarget() == m_listEnemyInWorld[i]->getBody()) {
						m_listBulletInWorld[j]->SetTarget(NULL);
					}
				}
				m_world->DestroyBody(m_listEnemyInWorld[i]->getBody());
				delete m_listEnemyInWorld[i]->getModel();
				delete m_listEnemyInWorld[i];
				m_listEnemyInWorld.erase(m_listEnemyInWorld.begin() + i);
				i--;
			}
		}

		for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
			m_listBulletInWorld[i]->Update(deltaTime);
			bool isContact = false;
			for (b2ContactEdge* edge = m_listBulletInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN || b->GetFilterData().categoryBits == CATEGORY_TERRAIN || a->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP || b->GetFilterData().categoryBits == CATEGORY_SLOW_TRAP) {
					RemoveBullet(i);
					isContact = true;
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER || b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					if (a->GetFilterData().categoryBits == CATEGORY_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							if (m_listBulletInWorld[i]->IsChange()) {
								SetStateHellGun(m_listBulletInWorld[i], a->GetAABB(0).GetExtents().x);
							}
						}
						else if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
							m_MainCharacter->SetHP(m_MainCharacter->GetHP() + m_listBulletInWorld[i]->GetAttackDame() * 0.1);
							float v = m_listBulletInWorld[i]->GetSpeedOfBullet().x > 0 ? 1 : -1;
							b2Vec2 posHellBullet = m_listBulletInWorld[i]->getBody()->GetPosition();
							Bullet* bullet = new Bullet(m_listBulletInWorld[i]->GetID());
							bullet->InitA(m_listBulletInWorld[i]->GetAttackDame(), m_listBulletInWorld[i]->GetAttackSpeed(), m_listBulletInWorld[i]->GetSpeedOfBullet().x, 0, m_listBulletInWorld[i]->GetMaxOfLength());
							bullet->SetCurrLength(m_listBulletInWorld[i]->GetCurrLength() + a->GetAABB(0).GetExtents().x);

							if (m_listBulletInWorld[i]->IsChange()) {
								bullet->SetIsChange();
							}

							Vector3 posBullet = Vector3(posHellBullet.x + v * (m_listBulletInWorld[i]->GetBox().x * 3 + a->GetAABB(0).GetExtents().x * 2), posHellBullet.y, 0);
							bullet->setModel(m_listBulletInWorld[i]->getModel());
							bullet->setShader(m_listBulletInWorld[i]->getShaders());
							bullet->SetTexture(m_listBulletInWorld[i]->getTexture());
							bullet->SetPosition(posBullet);
							bullet->SetScale(m_listBulletInWorld[i]->GetScale());
							bullet->SetRotation(m_listBulletInWorld[i]->GetRotation());
							bullet->InitWVP();
							bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
							bullet->m_current_anim = m_direction;

							AddBullet(bullet);
						}
					}
					if (b->GetFilterData().categoryBits == CATEGORY_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							if (m_listBulletInWorld[i]->IsChange()) {
								SetStateHellGun(m_listBulletInWorld[i], b->GetAABB(0).GetExtents().x);
							}
						}
						else if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
							m_MainCharacter->SetHP(m_MainCharacter->GetHP() + m_listBulletInWorld[i]->GetAttackDame() * 0.1);
							float v = m_listBulletInWorld[i]->GetSpeedOfBullet().x > 0 ? 1 : -1;
							b2Vec2 posHellBullet = m_listBulletInWorld[i]->getBody()->GetPosition();
							Bullet* bullet = new Bullet(m_listBulletInWorld[i]->GetID());
							bullet->InitA(m_listBulletInWorld[i]->GetAttackDame(), m_listBulletInWorld[i]->GetAttackSpeed(), m_listBulletInWorld[i]->GetSpeedOfBullet().x, 0, m_listBulletInWorld[i]->GetMaxOfLength());
							bullet->SetCurrLength(m_listBulletInWorld[i]->GetCurrLength() + b->GetAABB(0).GetExtents().x);

							Vector3 posBullet = Vector3(posHellBullet.x + v * (m_listBulletInWorld[i]->GetBox().x * 3 + b->GetAABB(0).GetExtents().x * 2), posHellBullet.y, 0);
							if (m_listBulletInWorld[i]->IsChange()) {
								bullet->SetIsChange();
							}
							bullet->setModel(m_listBulletInWorld[i]->getModel());
							bullet->setShader(m_listBulletInWorld[i]->getShaders());
							bullet->SetTexture(m_listBulletInWorld[i]->getTexture());
							bullet->SetPosition(posBullet);
							bullet->SetScale(m_listBulletInWorld[i]->GetScale());
							bullet->SetRotation(m_listBulletInWorld[i]->GetRotation());
							bullet->InitWVP();
							bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
							bullet->m_current_anim = -m_listBulletInWorld[i]->m_current_anim;

							AddBullet(bullet);
						}
					}
					isContact = true;
					RemoveBullet(i);
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || b->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || a->GetFilterData().categoryBits == CATEGORY_BULLET_BOSS || b->GetFilterData().categoryBits == CATEGORY_BULLET_BOSS) {
					if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
						m_MainCharacter->SetHP(m_MainCharacter->GetHP() - b->GetDensity());
						Camera::GetInstance()->is_wound = true;
					}
					if (b->GetFilterData().categoryBits == CATEGORY_PLAYER) {
						m_MainCharacter->SetHP(m_MainCharacter->GetHP() - a->GetDensity());
						Camera::GetInstance()->is_wound = true;
					}
					isContact = true;
					RemoveBullet(i);
					i--;
					break;
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
							m_listBulletInWorld[i]->ReverseV();
						}
					}
				}
			}

		}
	}

	if (pos.x > 18400 && m_IsBossAppear == false && m_IsTowerDefend == true) {
		if (m_boss) {
			if (!m_boss->isDie()) {
				m_IsBossAppear = true;
				m_boss->SetBodyObject(m_boss->GetPosition().x, m_boss->GetPosition().y, m_world);

				// set HP texture
				Model* modelHP = new Model();
				modelHP->InitSprite(0, 0, 1000, 50, 1000, 50, false);
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

				m_boss->SetHPTexture(whiteHp);
				m_boss->SetHPTexture(redHp, false);

				// set icon boss
				Model* modelIcon = new Model();
				modelIcon->InitSprite(0, 0, 150, 100, 150, 100, false);
				Healthy* icon = new Healthy(2);
				icon->setModel(modelIcon);
				icon->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
				icon->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(BOSS_ICON));
				icon->SetPosition(m_boss->GetPosition());
				icon->SetOriginScale(1, 1, 1);
				icon->SetRotation(m_boss->GetRotation());
				icon->InitWVP();

				m_boss->SetIcon(icon);
			}
		}
	}

	// victory
	if (m_IsTowerDefend == true) {
		if (Camera::GetInstance()->i_state < 4) {
			m_bIsVictory = true;
			m_bChangeScreen = true;
		}
		else {
			if (m_boss->isDie()) {
				m_bIsVictory = true;
				m_bChangeScreen = true;
			}
		}
	}


	// lose
	if (m_MainCharacter->isDie()) {
		m_MainCharacter->playDead(deltaTime);
		if (Camera::GetInstance()->is_dead == false) {
			for (int i = 0; i < (int)m_listEnemyInWorld.size(); i++) {
				m_listEnemyInWorld[i]->getBody()->SetEnabled(false);
			}
		}
		Singleton<GameplayUI>::GetInstance()->Update(deltaTime);
		int lop = deltaTime / 0.003;
		m_MainCharacter->getBody()->ApplyLinearImpulseToCenter(b2Vec2(0, 20000), true);
		for (int i = 0; i < lop; i++) {
			m_world->Step(0.07f, 6, 2);
			m_MainCharacter->Update(deltaTime);
		}
		return;
		m_bChangeScreen = true;
		m_bIsVictory = false;
	}

	//Change To Result Screen
	if (m_bChangeScreen) { //Check if Change Screen
		ChangeToResultScreen(m_bIsVictory); // m_bIsVictory default = false;
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
			if (numJump < 2) {
				jumpstep = 30;
				m_MainCharacter->resetAnimation(RunJump);
				m_MainCharacter->resetAnimation(Jump);
				m_MainCharacter->resetAnimation(Falling);
				++numJump;
				keyPressed = keyPressed | MOVE_JUMP;
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
		case 'n':
		case 'N':
			m_bIsVictory = true;
			m_bChangeScreen = true;
			break;
		case 'M':
		case 'm':
			m_bChangeScreen = true;
			break;
		}
	}
}

void SceneManager::CheckMovement() {
	if (!is_roll) {
		if (is_in_ground) m_MainCharacter->m_current_anim = Idle * m_direction;
		else if (jumpstep <= 0) m_MainCharacter->m_current_anim = Falling * m_direction;
		if (keyPressed & MOVE_RIGHT) {
			m_direction = 1;
			if (is_in_ground) m_MainCharacter->m_current_anim = Run;
			m_MainCharacter->resetAnimation(Idle);
			m_Horizontal = 40.0f;
		}
		else if (keyPressed & MOVE_LEFT) {
			m_direction = -1;
			if (is_in_ground) m_MainCharacter->m_current_anim = -Run;
			m_MainCharacter->resetAnimation(Idle);
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
	Vector3 mpos = m_MainCharacter->GetPosition();
	Vector3 epos = enemy->GetPosition();
	if (epos.x < mpos.x - 1280 || epos.x >(mpos.x + 1280)) return false;
	if (epos.y < mpos.y - 300 || epos.y > mpos.y + 300) return false;
	return true;
}