#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "define.h"
#include <iostream>
#include "Globals.h"


SceneManager::SceneManager(char* fileSM, char* fileMAP)
{
	m_fileSM = fileSM;
	m_fileMAP = fileMAP;
	m_direction = 1.0f;
	m_Horizontal = 0.0f;
	m_Vertical = 0.0f;
	m_shoot = 0.0f;
	m_time = 50.0f;
	m_timeChangeGun = 50.0f;
	keyPressed = 0;
}


SceneManager::~SceneManager()
{
}

void SceneManager::SetFileManager(char* fileSM, char* fileMAP) {
	m_fileSM = fileSM;
	m_fileMAP = fileMAP;
}

void SceneManager::Init() {
	jumpstep = 0;
	is_in_ground = false;
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
	FILE* f_MAP;
	f_MAP = fopen(m_fileMAP, "r+");
	if (f_MAP != NULL) {
		this->ReadMap(f_MAP);
	}

	// Init object
	if (m_MainCharacter != NULL) {
		m_MainCharacter->Init();
	}
	for (int i = 0; i < m_listEnemy.size(); i++) {
		m_listEnemy[i]->Init();
	}
	for (int i = 0; i < m_ListGun.size(); i++) {
		m_ListGun[i]->Init();
	}

	for (int i = 0; i < m_ListBackground.size(); i++) {
		m_ListBackground[i]->Init();
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
		fscanf(f_SM, "ROTATE_SPEED %f\n\n", &Rotate_Speed);
			
		Camera::GetInstance()->Init(Fovy, Near, Far, Move_Speed, Rotate_Speed);
		Camera::GetInstance()->SetPosition(Position);
		Camera::GetInstance()->SetTarget(Target);
		Camera::GetInstance()->SetUp(Up);
	}

	//Object
	int numOfObjects;
	fscanf(f_SM, "#Objects: %d\n", &numOfObjects);
	for (register int i = 0; i < numOfObjects; i++) {
		int ID, model, texture, shader, anim;
		char type[128];
		Vector3 Position, Rotation, Scale;

		fscanf(f_SM, "ID %d\n", &ID);
		std::cout << "ID: " << ID << std::endl;
		fscanf(f_SM, "TYPE %s\n", type);
		Model* pModel;
		float x, y, w, h, tw, th; char aniFile[128];
		float dame, attack, speedx, speedy, dis;
		fscanf(f_SM, "COORD %f %f %f %f %f %f\n", &x, &y, &w, &h, &tw, &th);
		pModel = new Model();
		pModel->InitSprite(x, y, w, h, tw, th);
		int num_anim;
		fscanf(f_SM, "ANIMATIONS %d\n", &num_anim);
		if (num_anim > 0) {
			pModel->b_IsAnimation = true;
			for (int i = 0; i < num_anim; i++) {
				
				fscanf(f_SM, "ANIMATION %d\n", &anim);
				pModel->addAnimation(ResourceManager::GetInstance()->GetAnimationAtID(anim));
			}
		}
		fscanf(f_SM, "SHADER %d\n", &shader);
		fscanf(f_SM, "TEXTURE %d\n", &texture);

		if (strcmp(type, "GUN") == 0) {
			fscanf(f_SM, "BULLET %f %f %f %f %f\n", &dame, &attack, &speedx, &speedy, &dis);
		}
			//Add Texture here
		
		fscanf(f_SM, "POSITION %f %f %f\n", &Position.x, &Position.y, &Position.z);
		fscanf(f_SM, "ROTATION %f %f %f\n", &Rotation.x, &Rotation.y, &Rotation.z);
		fscanf(f_SM, "SCALE %f %f %f\n\n", &Scale.x, &Scale.y, &Scale.z);
		

		if (strcmp(type, "MAIN") == 0) {
			m_MainCharacter = new MainCharacter(ID);
			m_MainCharacter->setModel(pModel);
			m_MainCharacter->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			m_MainCharacter->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			m_MainCharacter->SetPosition(Position);
			m_MainCharacter->SetScale(Scale);
			m_MainCharacter->SetRotation(Rotation);
			m_MainCharacter->SetHP(1000);
			m_MainCharacter->SetBodyObject(m_MainCharacter->GetPosition().x, m_MainCharacter->GetPosition().y, m_world);
			m_MainCharacter->InitWVP();
		}
		else if (strcmp(type, "NPC") == 0) {
			Enemy* enemy = new Enemy(ID);
			enemy->setModel(pModel);
			enemy->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
			enemy->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
			enemy->SetPosition(Position);
			enemy->SetScale(Scale);
			enemy->SetRotation(Rotation);
			enemy->SetHP(1000);
			enemy->SetBodyObject(enemy->GetPosition().x, enemy->GetPosition().y, m_world);
			enemy->InitWVP();
			AddEnemy(enemy);
		}
		else if (strcmp(type, "GUN") == 0) {
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
		else {
			// do something
		}
	}
	
	fclose(f_SM);
}

void SceneManager::ReadMap(FILE *f_MAP) {
	int width, height, row, col, x, c, num, index;

	fscanf(f_MAP, "%d %d\n", &width, &height);
	fscanf(f_MAP, "%d\n", &index);

	fscanf(f_MAP, "%d %d\n", &row, &col);
	Model* terrainModel = new Model();
	terrainModel->InitSprite(0, 0, WIDTH, WIDTH, WIDTH, WIDTH);

	for (int i = 0; i < row; i++) {
		std::vector<int> line;
		std::vector<int> isLine;
		std::vector<Terrain*> lineMap;
		for (int j = 0; j < col; j++) {
			fscanf(f_MAP, "%d ", &x);
			line.push_back(x);
			Terrain* terrain = new Terrain(x);
			if (x >= 0) {
				terrain->setModel(terrainModel);
				terrain->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
				terrain->SetTexture(ResourceManager::GetInstance()->GetTerrainAtID(x));
				terrain->SetPosition(WIDTH *(j - col / 2), WIDTH*(i - row / 2), 0);
				terrain->SetScale(Vector3(1, 1, 1));
				terrain->SetRotation(Vector3(0, 0, 0));
				terrain->SetBodyObject(WIDTH, WIDTH, m_world);
				terrain->InitWVP();
			}
			isLine.push_back(0);
			lineMap.push_back(terrain);
		}
		isInit.push_back(isLine);
		map.push_back(line);
		m_listTerrain.push_back(lineMap);
	}

	num = (col * height) / (2 * width * row) + 1;
	num = 2 * num + 1;
	int n = WIDTH / 13;
	Model* backgroundModel = new Model();
	backgroundModel->InitSprite(0, 0, n * (width * col / height), n * col, n * (width * col / height), n * col);

	for (int i = 0; i < num; i++) {
		Terrain *background = new Terrain(0);
		background->setModel(backgroundModel);
		background->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
		background->SetTexture(ResourceManager::GetInstance()->GetBackgroundAtID(index));
		background->SetPosition(n * (width * col / height) * (i - num / 2), 0, 0);
		background->SetScale(Vector3(1, 1, 1));
		background->SetRotation(Vector3(0, 0, 0));
		background->InitWVP();
		m_ListBackground.push_back(background);
	}
	fclose(f_MAP);
}


void SceneManager::Draw() {

	//for (int i = 0; i < (int) m_ListTerrain.size(); i++) {
		//m_ListTerrain[i]->Draw();
	//}
	m_MainCharacter->Draw();


	for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->Draw();
	}

	for (int i = 0; i < (int) m_listEnemy.size(); i++) {
		m_listEnemy[i]->Draw();
	}

	b2Vec2 pos = m_MainCharacter->getBody()->GetPosition();
	int col = Globals::screenWidth / WIDTH * 2 + 1;
	int row = Globals::screenHeight / WIDTH * 2 + 1;
	int w = pos.x / WIDTH + m_listTerrain[0].size() / 2;
	int h = 0 / WIDTH + m_listTerrain.size() / 2;
	int wlow = w - col > 0 ? w - col : 0;
	int whigh = w + col < m_listTerrain[0].size() ? w + col : m_listTerrain[0].size() - 1;
	int hlow = h - row > 0 ? h - row : 0;
	int hhigh = h + row < m_listTerrain.size() ? h + row : m_listTerrain.size() - 1;
	for (int i = hlow; i < hhigh; i++) {
		for (int j = wlow; j < whigh; j++) {
			if (map[i][j] >= 0) {
				if (isInit[i][j] == 0) {
					m_listTerrain[i][j]->Init();
					isInit[i][j] = 1;
				}
				m_listTerrain[i][j]->Draw();
			}
		}
	}

	for (int i = 0; i < m_ListBackground.size(); i++) {
		m_ListBackground[i]->Draw();
	}
}


void SceneManager::AddGun(Bullet* gun) {
	m_ListGun.push_back(gun);
}

void SceneManager::AddBullet(Bullet* bullet) {
	m_listBulletInWorld.push_back(bullet);
}

void SceneManager::RemoveBullet(int index) {
	m_world->DestroyBody(m_listBulletInWorld[index]->getBody());
//	m_listBulletInWorld[index]->CleanUp();
	m_listBulletInWorld.erase(m_listBulletInWorld.begin() + index);

//	printf("%d\n", m_listBulletInWorld.size());

}

void SceneManager::AddEnemy(Enemy* enemy) {
	m_listEnemy.push_back(enemy);
}



void SceneManager::OnMouseButtonDown(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
		SetIsFighting(true);
		m_oTarget = Vector2(X, Y);
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
		SetIsFighting(false);
		m_oTarget = Vector2(X, Y);
	}
	break;
	case RMB:
	{
	}
	break;
	}
}

void SceneManager::OnMouseButtonMove(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
	{
		m_oTarget = Vector2(X, Y);
	}
	break;
	case RMB:
	{
	}
	break;
	}
}

void SceneManager::SetIsFighting(bool IsFighting) {
	m_bIsFighting = IsFighting;
}

void SceneManager::CleanUp() {
	Camera::GetInstance()->CleanUp();
	for (int i = 0; i < m_listTerrain.size(); i++) {
		for (int j = 0; j < m_listTerrain[i].size(); j++) {
			m_listTerrain[i][j]->CleanUp();
		}
	}

	m_MainCharacter->CleanUp();

	for (int i = 0; i < (int)m_listEnemy.size(); i++) {
		m_listEnemy[i]->CleanUp();

	}

	for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
//		m_listBulletInWorld[i]->CleanUp();
	}
}

void SceneManager::Shoot() {
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	Bullet* bullet = new Bullet(m_ListGun[0]->GetID());
	bullet->InitA(m_ListGun[0]->GetAttackDame(), m_ListGun[0]->GetAttackSpeed(), m_direction*m_ListGun[0]->GetSpeedOfBullet().x, m_ListGun[0]->GetSpeedOfBullet().y, m_ListGun[0]->GetMaxOfLength());
	Vector3 posBullet = Vector3(posMainCharacter.x + m_direction * (m_MainCharacter->GetBox().x + m_ListGun[0]->GetBox().x / 2), posMainCharacter.y, 0);

	bullet->SetIsChange();
	bullet->setModel(m_ListGun[0]->getModel());
	bullet->setShader(m_ListGun[0]->getShaders());
	bullet->SetTexture(m_ListGun[0]->getTexture());
	bullet->SetPosition(posBullet);
	bullet->SetScale(m_ListGun[0]->GetScale());
	bullet->SetRotation(m_ListGun[0]->GetRotation());
	bullet->InitWVP();
	bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
	bullet->Init();
	
	AddBullet(bullet);
}

void SceneManager::ChangeGun(bool isEmptyBullet) {
	Bullet *bullet = m_ListGun[0];
	if (isEmptyBullet) {
		bullet->ResetBullet();
		m_ListGun.erase(m_ListGun.begin());
		m_ListGun.push_back(bullet);
	}
	else {
		m_ListGun[0] = m_ListGun[1];
		m_ListGun[1] = bullet;
	}
}

void SceneManager::SetStateHellGun(Bullet* hellBullet, float enemyWidth) {
	float v = hellBullet->GetSpeedOfBullet().x > 0 ? 1 : -1;
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
		bullet->Init();

		AddBullet(bullet);
	}
}
void SceneManager::Update(float deltaTime) {
	// set key
	m_time += deltaTime;
	m_timeChangeGun += deltaTime;
	CheckMovement();


	// set update
	m_MainCharacter->getBody()->SetFixedRotation(true);
	// set v
	if (jumpstep > 0) {
		float impulse = m_MainCharacter->getBody()->GetMass() * 5;
		float impulseX = m_Horizontal * 70;
		m_MainCharacter->getBody()->ApplyLinearImpulse(b2Vec2(impulseX, -impulse), m_MainCharacter->getBody()->GetWorldCenter(), true);
		jumpstep--;
	}
	else {
		m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(m_Horizontal, 45.0));
	}

	int32 velocityIterations = 2;
	int32 positionIterations = 1;
	int lop = deltaTime / 0.003f;

	for(int iLop = 0;iLop < lop;iLop++){
		m_world->Step(0.05f, velocityIterations, positionIterations); 
		m_MainCharacter->Update(deltaTime);

		is_in_ground = false;
		for (b2ContactEdge* edge = m_MainCharacter->getBody()->GetContactList(); edge != NULL; edge = edge->next) {
			b2Fixture * a = edge->contact->GetFixtureA();
			b2Fixture * b = edge->contact->GetFixtureB();
			if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN || b->GetFilterData().categoryBits == CATEGORY_TERRAIN) is_in_ground = true;
		}

		for (int i = 0; i < (int)m_listEnemy.size(); i++) {
			m_listEnemy[i]->Update(deltaTime);
			for (b2ContactEdge* edge = m_listEnemy[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - b->GetDensity());
				}
				if (b->GetFilterData().categoryBits == CATEGORY_PLAYER) {
					m_MainCharacter->SetHP(m_MainCharacter->GetHP() - a->GetDensity());
				}
				if (a->GetFilterData().maskBits == MASK_BULLET_PLAYER) {
					m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - a->GetDensity());
				}
				if (b->GetFilterData().maskBits == MASK_BULLET_PLAYER) {
					m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - b->GetDensity());
				}
			}
		}

		for (int i = 0; i < (int)m_listBulletInWorld.size(); i++) {
			m_listBulletInWorld[i]->Update(deltaTime);
			bool isContact = false;
			for (b2ContactEdge* edge = m_listBulletInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_TERRAIN || b->GetFilterData().categoryBits == CATEGORY_TERRAIN) {
					RemoveBullet(i);
					isContact = true;
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER || b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					if (a->GetFilterData().categoryBits == CATEGORY_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							printf("hell %d\n", i);
							if (m_listBulletInWorld[i]->IsChange()) {
								SetStateHellGun(m_listBulletInWorld[i], a->GetAABB(0).GetExtents().x);
							}
						}
						else if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
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
							bullet->Init();

							AddBullet(bullet);
						}
					}
					if (b->GetFilterData().categoryBits == CATEGORY_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							printf("hell %d\n", i);
							if (m_listBulletInWorld[i]->IsChange()) {
								SetStateHellGun(m_listBulletInWorld[i], b->GetAABB(0).GetExtents().x);
							}
						}
						else if (m_listBulletInWorld[i]->GetID() == CATEGORY_BOOMERANG) {
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
							bullet->Init();

							AddBullet(bullet);
						}
					}
					isContact = true;
					RemoveBullet(i);
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || b->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY) {
					if (a->GetFilterData().categoryBits == CATEGORY_PLAYER) {
						m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - a->GetDensity());
					}
					if (b->GetFilterData().maskBits == CATEGORY_PLAYER) {
						m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - b->GetDensity());
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
}

void SceneManager::Key(unsigned char key, bool isPressed) {
	if (isPressed) {
		switch (key)
		{
		case KEY_LEFT:
		case KEY_LEFT + 32:
			m_direction = -1.0f;
			keyPressed = keyPressed | MOVE_LEFT;
			break;
		case KEY_RIGHT:
		case KEY_RIGHT + 32:
			m_direction = 1.0f;
			keyPressed = keyPressed | MOVE_RIGHT;
			break;
		case KEY_JUMP:
		case KEY_JUMP + 32:
			jumpstep = 20;
			m_MainCharacter->resetAnimation(RunJump);
			keyPressed = keyPressed | MOVE_JUMP;
			break;
		case KEY_CHANGE_GUN:
		case KEY_CHANGE_GUN + 32:
			keyPressed = keyPressed | CHANGE_GUN;
			break;
		case KEY_SHOOT:
		case KEY_SHOOT + 32:
			keyPressed = keyPressed | SHOOT;
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
			m_Vertical = 39.8f;
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
			break;
		}
	}
}

void SceneManager::CheckMovement() {
	if (is_in_ground) m_MainCharacter->m_current_anim = Idle * m_direction;
	else if (jumpstep <= 0) m_MainCharacter->m_current_anim = Falling * m_direction;
	if (keyPressed & MOVE_RIGHT) {
		m_direction = 1.0f;
		if (is_in_ground) m_MainCharacter->m_current_anim = Run;
		m_Horizontal = 20.0f;
	}
	else if (keyPressed & MOVE_LEFT) {
		m_direction = -1.0f;
		if (is_in_ground) m_MainCharacter->m_current_anim = Run * m_direction;
		m_Horizontal = -20.0f;
	}
	if (keyPressed & MOVE_JUMP) {
		if (m_Horizontal == 0) m_MainCharacter->m_current_anim = Jump * m_direction;
		else {
			m_MainCharacter->m_current_anim = RunJump * m_direction;
		}
	}
	if (keyPressed & SHOOT) {
		m_shoot = 1.0f;
		if (m_time >= m_ListGun[0]->GetAttackSpeed()) {
			Shoot();
			m_time = 0;
			if (m_ListGun[0]->IsEmptyBullet()) {
				ChangeGun(true);
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
	
}