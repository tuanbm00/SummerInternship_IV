#include "GameStateBase.h"
class GameplayUI :
	public GameStateBase
{
	std::shared_ptr<ButtonManager> m_BM;
	MainCharacter* m_MainCharacter;
	Boss* m_Boss;
	bool m_pBossAppear = false;
	float m_NumberOfBullets = 0;
	
public:
	GameplayUI(void);
	~GameplayUI(void);

	void Init();
	void Update(float deltaTime);
	void CleanUp();

	void Pause();
	void Resume();

	void HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed);
	void OnMouseButtonDown(int X, int Y, char Button);
	void OnMouseButtonUp(int X, int Y, char Button);
	void OnMouseButtonMove(int X, int Y, char Button);
	void OnMouseMoving(int X, int Y);

	void Draw();

	void SetMainCharacter(MainCharacter* mainCharacter);
	void SetBoss(Boss* boss);
	void SetBossAppear(bool IsAppear);
	void SetNumberOfBullets(int numberOfBullets);
};
