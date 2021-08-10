#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "define.h"
#include <iostream>



SceneManager::SceneManager(char* fileSM)
{
	m_fileSM = fileSM;
	m_direction = 1.0f;
	m_Horizontal = 0.0f;
	m_Vertical = 0.0f;
	m_shoot = 0.0f;
	m_time = 0.0f;
	keyPressed = 0;
	printf("init key hozinol\n");
}


SceneManager::~SceneManager()
{
}

void SceneManager::SetFileManager(char* fileSM) {
	m_fileSM = fileSM;
}

void SceneManager::Init() {
	jumpstep = 0;
	printf("Init state: %d\n", Camera::GetInstance()->i_state);
	//Box2D
	b2Vec2 gravity = b2Vec2(0.0, 0.0);
	m_world = new b2World(gravity);
	

	//resource
	ResourceManager::GetInstance()->Init();
	printf("Init resources done!\n");
	FILE* f_SM;
	f_SM = fopen(m_fileSM, "r+");
	if (f_SM != NULL) {
		this->ReadFile(f_SM);
	}
	else std::cout << "Khong tim thay file Scene Manager! \n";

	// Init object
	printf("Init sm done!\n");
	if (m_MainCharacter != NULL) {
		m_MainCharacter->Init();
	}
	for (int i = 0; i < m_listEnemy.size(); i++) {
		m_listEnemy[i]->Init();
	}
	for (int i = 0; i < m_ListGun.size(); i++) {
		m_ListGun[i]->Init();
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
		fscanf(f_SM, "COORD %f %f %f %f %f %f\n", &x, &y, &w, &h, &tw, &th);
		pModel = new Model();
		pModel->InitSprite(x, y, w, h, tw, th);
		printf("%f %f %f %f\n", x, y, w, h);
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
		printf("shader %d\n", shader);
		fscanf(f_SM, "TEXTURE %d\n", &texture);
		printf("tex %d\n", texture);
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
			bullet->InitA(50, 50, 50, 50, 0, 500);
			bullet->InitWVP();
			AddGun(bullet);
		}
		else {
			// do something
		}
	}
	
	fclose(f_SM);
}


/*void SceneManager::Update(float deltaTime) {
>>>>>>> Stashed changes
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
}*/

void SceneManager::Draw() {

	//for (int i = 0; i < (int) m_ListTerrain.size(); i++) {
		//m_ListTerrain[i]->Draw();
	//}
	m_MainCharacter->Draw();


	for (int i = 0; i < (int) m_listEnemy.size(); i++) {
		m_listEnemy[i]->Draw();
	}

	for (int i = 0; i < (int) m_listBulletInWorld.size(); i++) {
		m_listBulletInWorld[i]->Draw();
	}
}


void SceneManager::AddTerrain(Ground * obj) {
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

void SceneManager::Shoot() {
	b2Vec2 posMainCharacter = m_MainCharacter->getBody()->GetPosition();
	Bullet* bullet = new Bullet(m_ListGun[0]->GetID());
	bullet->InitA(0, bullet->GetAttackDame(), bullet->GetAttackSpeed(), m_direction*bullet->GetSpeedOfBullet().x, bullet->GetSpeedOfBullet().y, bullet->GetMaxOfLength());
	Vector3 posBullet = Vector3(posMainCharacter.x + m_direction * (m_MainCharacter->GetBox().x + m_ListGun[0]->GetBox().x / 2), posMainCharacter.y, 0);
	
	// thieu phan doc textrue cua anh
	
	bullet->SetPosition(posBullet);
	bullet->SetRotation(m_ListGun[0]->GetRotation());
	bullet->SetScale(m_ListGun[0]->GetScale());

	bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
	m_listBulletInWorld.push_back(bullet);
}

void SceneManager::ChangeGun(bool isEmptyBullet ) {
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

void SceneManager::SetStateHellGun(Bullet* hellBullet, float enemyWidth) {
	for (int i = 0; i < 3; i++) {
		b2Vec2 posHellBullet = hellBullet->getBody()->GetPosition();
		Bullet* bullet = new Bullet(hellBullet->GetID());
		bullet->InitA(0, hellBullet->GetAttackDame(), hellBullet->GetAttackSpeed(), m_direction*hellBullet->GetSpeedOfBullet().x, hellBullet->GetSpeedOfBullet().x / 2 * (i - 2), hellBullet->GetMaxOfLength());
		Vector3 posBullet = Vector3(posHellBullet.x + m_direction * (hellBullet->GetBox().x + enemyWidth), posHellBullet.y, 0);

		// thieu phan doc textrue cua anh

		bullet->SetPosition(posBullet);
		bullet->SetRotation(m_ListGun[0]->GetRotation());
		bullet->SetScale(m_ListGun[0]->GetScale());

		bullet->SetBodyObject(posBullet.x, posBullet.y, m_world);
		m_listBulletInWorld.push_back(bullet);
	}
}

void SceneManager::Update(float deltaTime) {
	CheckMovement();
	// set v
	if (jumpstep > 0) {
		m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(m_Horizontal, -50000));
		jumpstep--;
	}
	else m_MainCharacter->getBody()->SetLinearVelocity(b2Vec2(m_Horizontal, m_Vertical));

	int32 velocityIterations = 2;
	int32 positionIterations = 1;
	int lop = deltaTime / 0.001f;
	for(int i = 0;i < lop;i++){
		m_world->Step(0.05f, velocityIterations, positionIterations); // sua cai nay cho hop ly la dc uk xoa tat may cai in ra nam hinh di do chamnuk

		m_MainCharacter->Update(deltaTime);

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
			for (b2ContactEdge* edge = m_listBulletInWorld[i]->getBody()->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().maskBits == MASK_TERRAIN || b->GetFilterData().maskBits == MASK_TERRAIN) {
					RemoveBullet(i);
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER || b->GetFilterData().categoryBits == CATEGORY_BULLET_PLAYER) {
					if (a->GetFilterData().maskBits == MASK_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							SetStateHellGun(m_listBulletInWorld[i], a->GetAABB(0).GetExtents().x);
						}
					}
					if (b->GetFilterData().maskBits == MASK_ENEMY) {
						if (m_listBulletInWorld[i]->GetID() == CATEGORY_HELL_GUN) {
							SetStateHellGun(m_listBulletInWorld[i], b->GetAABB(0).GetExtents().x);
						}
					}
					RemoveBullet(i);
					i--;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY || b->GetFilterData().categoryBits == CATEGORY_BULLET_ENEMY) {
					if (a->GetFilterData().maskBits == MASK_BULLET_ENEMY) {
						m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - a->GetDensity());
					}
					if (b->GetFilterData().maskBits == MASK_BULLET_ENEMY) {
						m_listEnemy[i]->SetHP(m_listEnemy[i]->GetHP() - b->GetDensity());
					}
					RemoveBullet(i);
					i--;
					break;
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
			keyPressed = keyPressed | MOVE_LEFT;
			break;
		case KEY_RIGHT:
		case KEY_RIGHT + 32:
			keyPressed = keyPressed | MOVE_RIGHT;
			break;
		case KEY_JUMP:
		case KEY_JUMP + 32:
			jumpstep = 20;
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
		m_MainCharacter->m_current_anim = Idle;
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
			m_Vertical = 9.8f;
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
	if (keyPressed & MOVE_RIGHT) {
		m_MainCharacter->m_current_anim = RunFW;
		m_Horizontal = 1024.0f;
		m_direction = 1.0f;
	}
	if (keyPressed & MOVE_LEFT) {
		m_MainCharacter->m_current_anim = RunBW;
		m_Horizontal = -1024.0f;
		m_direction = -1.0f;
	}
	if (keyPressed & MOVE_JUMP) {
		//m_Vertical = -90.8f;
	}
	if (keyPressed & SHOOT) {
		m_shoot = 1.0f;
	}
	if (keyPressed & CHANGE_GUN) {
		m_changeGun = 1.0f;
	}
	
}